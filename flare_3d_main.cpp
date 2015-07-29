

#include <flare_gui/flare_gui.h>
#include <allegro_flare/model.h>
#include <allegro5/allegro_opengl.h>



// In theory, the Shader and the Camera should be in AllegroFlare


class Shader
{
private:
	ALLEGRO_SHADER *shader;

public:
	Shader(const char *vertex_source_filename, const char *fragment_source_filename)
		// for now, shaders will be GLSL in AllegroFlare.  This should eventually change in the future
		: shader(al_create_shader(ALLEGRO_SHADER_GLSL))
	{
		if (!shader) std::cerr << "There was a problem creating a shader." << std::endl;

		if (!al_attach_shader_source_file(shader, ALLEGRO_VERTEX_SHADER, vertex_source_filename))
			std::cerr << "There was an error attaching the VERTEX shader source from file:"
						<< std::endl << al_get_shader_log(shader) << std::endl;

		if (!al_attach_shader_source_file(shader, ALLEGRO_PIXEL_SHADER, fragment_source_filename))
			std::cerr << "There was an error attaching the FRAGMENT shader source from file:"
						<< std::endl << al_get_shader_log(shader) << std::endl;

		if (!al_build_shader(shader))
		{
			std::cerr << "There were errors when building the shader:" << std::endl;
			std::cerr << al_get_shader_log(shader) << std::endl;
		}
	}
	~Shader()
	{
		al_destroy_shader(shader);
	}

	// activate and deactivate

	void use()
	{
		al_use_shader(shader);
	}
	static void stop()
	{
		al_use_shader(NULL);
	}

	// set uniforms and attributes (these apply to the /currently active/ shader)

	static bool set_mat4(const char *name, ALLEGRO_TRANSFORM *t)
	{
		return al_set_shader_matrix(name, t);
	}
	static bool set_int(const char *name, int i)
	{
		return al_set_shader_int(name, i);
	}
	static bool set_float(const char *name, float f)
	{
		return al_set_shader_float(name, f);
	}
	static bool set_bool(const char *name, bool b)
	{
		return al_set_shader_bool(name, b);
	}
	static bool set_vec3(const char *name, float x, float y, float z)
	{
		float vec3[3] = {x, y, z};
		return al_set_shader_float_vector(name, 3, &vec3[0], 1);
	}
	static bool set_vec3(const char *name, const vec3d vec)
	{
		set_vec3(name, vec.x, vec.y, vec.z);
	}
};



class Camera3
{
public:
	vec3d stepout;
	vec3d position;
	float spin;
	float tilt;
	Camera3()
		: stepout(0, 0, 0)
		, position(0, 0, 0)
		, spin(0)
		, tilt(0)
	{}
	void position_transform(ALLEGRO_TRANSFORM *t)
	{
		al_identity_transform(t);

		al_translate_transform_3d(t, stepout.x, stepout.y, stepout.z);
		al_rotate_transform_3d(t, -1, 0, 0, tilt);
		al_rotate_transform_3d(t, 0, -1, 0, spin);
	}
	void reverse_position_transform(ALLEGRO_TRANSFORM *t)
	{
		// note: this is EXACTLY the same as position transform, except the
		// order of transformations is reversed, and the values are negated
		al_identity_transform(t);

		al_rotate_transform_3d(t, 0, 1, 0, spin);
		al_rotate_transform_3d(t, 1, 0, 0, tilt);
		al_translate_transform_3d(t, -stepout.x, -stepout.y, -stepout.z);
	}
	vec3d get_real_position()
	{
		vec3d real_position(0, 0, 0);
		ALLEGRO_TRANSFORM t;

		position_transform(&t);
		al_transform_coordinates_3d(&t, &real_position.x, &real_position.y, &real_position.z);

		return real_position;
	}
};



class Light
{
public:
	vec3d position;
	Light(float x, float y, float z)
		: position(x, y, z)
	{}
};



class My3DProject : public Screen
{
public:
	// bins
	BitmapBin bitmaps;

	// assets
	Camera3 camera;
	ModelNew construct;
	ModelNew model;
	ModelNew unit_sphere;
	Shader metalic_shader;
	Shader fresnel_shader;
	Light light;

	// state
	bool camera_spinning;
	float model_scale;


	My3DProject(Display *display)
		: Screen(display)
		, bitmaps()
		, camera()
		, construct()
		, model()
		, unit_sphere()
		, metalic_shader("data/shaders/vertex.glsl", "data/shaders/fragment.glsl")
		, fresnel_shader("data/shaders/fresnel_vertex.glsl", "data/shaders/fresnel_fragment.glsl")
		, light(4, 4, 3)
		, camera_spinning(true)
		, model_scale(0.4)
	{
		camera.stepout = vec3d(0, 1.5, 4);

		construct.load_obj_file("data/models/construct-beta-01.obj");
		construct.set_texture(bitmaps["uv.png"]);

		model.load_obj_file("data/models/allegro_flare_unit_logo-04.obj");

		unit_sphere.load_obj_file("data/models/unit_sphere4.obj", 0.2);
	}
	void setup_projection()
	{
		ALLEGRO_TRANSFORM t;

		camera.reverse_position_transform(&t);
	
		float aspect_ratio = (float)al_get_bitmap_height(backbuffer_sub_bitmap) / al_get_bitmap_width(backbuffer_sub_bitmap);
		al_perspective_transform(&t, -1, aspect_ratio, 1, 1, -aspect_ratio, 1000);

		//al_set_target_bitmap(backbuffer_sub_bitmap); << I don't think this is necessairy, it also occours just prior to this function
		al_use_projection_transform(&t);
	}
	void primary_timer_func() override
	{
		// spin the camera
		if (camera_spinning) camera.spin += 1.0 / 60.0 * 0.1;


		// setup the render settings
		al_set_render_state(ALLEGRO_DEPTH_TEST, 1);
		al_set_render_state(ALLEGRO_WRITE_MASK, ALLEGRO_MASK_DEPTH | ALLEGRO_MASK_RGBA);
		al_clear_depth_buffer(1);


		// the functionality of this line might eventually be embedded in the framework
		setup_projection();


		// draw the Construct (the environment box)
		glEnable(GL_CULL_FACE); // requiring opengl should eventually be fazed out
		construct.draw();
		glDisable(GL_CULL_FACE);


		// draw our (rotating) model in the center of the world
		ALLEGRO_TRANSFORM t;
		al_identity_transform(&t);
		al_rotate_transform_3d(&t, 1, 0, 0, FULL_ROTATION*0.25);
		al_rotate_transform_3d(&t, 0, 1, 0, al_get_time()*0.5);
		al_scale_transform_3d(&t, model_scale + 0.5, model_scale + 0.5, model_scale + 0.5);
		al_translate_transform_3d(&t, 0, 1.5, 0);
		//al_use_transform(&t); < note: this is not needed because transform information is processed in the shader

		metalic_shader.use();
		metalic_shader.set_vec3("camera_position", camera.get_real_position());
		metalic_shader.set_vec3("light_position", light.position);
		metalic_shader.set_mat4("position_transform", &t);

		model.draw();


		// draw the light (so we now where it is)
		al_identity_transform(&t);
		al_translate_transform_3d(&t, light.position.x, light.position.y, light.position.z);

		fresnel_shader.use();
		metalic_shader.set_vec3("camera_position", camera.get_real_position());
		metalic_shader.set_vec3("light_position", light.position);
		metalic_shader.set_mat4("position_transform", &t);

		unit_sphere.draw();


		// restore the identity transform before the display gets flipped (and turn off shaders for good measure)
		al_identity_transform(&t);
		al_use_transform(&t);
		Shader::stop();
	}
};



class MyGUIScreen : public FGUIScreen
{
private:
	My3DProject *scene;
	FGUIButton *button;
	FGUIDial *dial;

public:
	MyGUIScreen(Display *display, My3DProject *scene)
		: FGUIScreen(display)
		, scene(scene)
		, button(new FGUIButton(this, 110, display->height()-70, 160, 50, "camera spin"))
		, dial(new FGUIDial(this, 240, display->height()-70, 60))
	{
		dial->set_value(scene->model_scale);
	}
	void on_message(FGUIWidget *sender, std::string message) override
	{
		if (sender == button) scene->camera_spinning = !scene->camera_spinning;

		if (sender == dial) scene->model_scale = static_cast<FGUIDial *>(sender)->get_value();
	}
};



int main(int argc, char **argv)
{
	af::initialize();
	Display *display = af::create_display(960, 600, ALLEGRO_OPENGL | ALLEGRO_PROGRAMMABLE_PIPELINE);
	My3DProject *proj = new My3DProject(display);
	MyGUIScreen *gui = new MyGUIScreen(display, proj);
	af::run_loop();
}


