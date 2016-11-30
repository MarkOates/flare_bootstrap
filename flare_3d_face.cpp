

#include <allegro_flare/allegro_flare.h>
#include <allegro5/allegro_opengl.h>

#include <allegro_flare/cubemap.h>



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
	//ModelBin models;

	// assets
	Camera3 camera;
	Model3D construct;
	Model3D model;
	Model3D *current_model;
	Model3D unit_sphere;
	ALLEGRO_FLARE_CUBEMAP_TEXTURE *cube_map;
	Shader metalic_shader;
	Shader fresnel_shader;
	Shader cubemap_shader;
	Shader multi_shader;
	Shader *current_shader;
	Light light;

	// params
	float model_scale;


	My3DProject(Display *display)
		: Screen(display)
		, bitmaps()
		//, models("data/models")
		, camera()
		, construct()
		, model()
		, current_model(NULL)
		, unit_sphere()
		, metalic_shader("data/shaders/metalic_vertex.glsl", "data/shaders/metalic_fragment.glsl")
		, fresnel_shader("data/shaders/fresnel_vertex.glsl", "data/shaders/fresnel_fragment.glsl")
		, cubemap_shader("data/shaders/cube_vertex.glsl", "data/shaders/cube_fragment.glsl")
		, multi_shader("data/shaders/multi_vertex.glsl", "data/shaders/multi_fragment.glsl")
		, current_shader(NULL)
		, light(4, 4, 3)
		, model_scale(0.08)
		, cube_map(NULL)
	{
		camera.stepout = vec3d(0, 1.5, 4);

		cube_map = glsl_create_cubemap_from_vertical_strip("data/bitmaps/sky4.png");

		construct.load_obj_file("data/models/construct-beta-02.obj");
		construct.set_texture(bitmaps["uv.png"]);

		//model.load_obj_file("data/models/allegro_flare_logo-02.obj");
		//model.load_obj_file("data/models/unit_sphere4.obj", 4.0);
		//model.load_obj_file("data/models/male_head.obj", 0.25);
		//model.load_obj_file("data/models/car2-04.obj", 1.0);
		//model.set_texture(bitmaps["car_texture.png"]);

		unit_sphere.load_obj_file("data/models/unit_sphere4.obj", 0.2);

		//current_model = models["male_had.obj"];
		

		set_model("male_head.obj");
		set_shader("metalic");
	}
	void set_model(std::string filename)
	{
		//model.load_obj_file("data/models/male_head.obj", 0.25);
		std::string full_filename = std::string("data/models/") + filename;
		if (current_model) delete current_model;
		current_model = new Model3D();
		current_model->load_obj_file(full_filename.c_str(), 0.25);
	}
	void set_shader(std::string shader_identifier)
   {
		current_shader = NULL;
		if (shader_identifier == "metalic") current_shader = &metalic_shader;
		if (shader_identifier == "fresnel") current_shader = &fresnel_shader;
		if (shader_identifier == "cubemap") current_shader = &cubemap_shader;
		if (shader_identifier == "multi") current_shader = &multi_shader;
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

		ALLEGRO_TRANSFORM t;
		al_identity_transform(&t);


		// draw our (rotating) model in the center of the world
		//al_rotate_transform_3d(&t, 1, 0, 0, FULL_ROTATION*0.25);
		al_rotate_transform_3d(&t, 0, 1, 0, al_get_time()*0.5);
		al_scale_transform_3d(&t, model_scale + 0.1, model_scale + 0.1, model_scale + 0.1);
		al_translate_transform_3d(&t, 0, 1.5, 0);
		//al_use_transform(&t); < note: this is not needed because transform information is processed in the shader


		if (current_shader) current_shader->use();
		//metalic_shader.use();
		//multi_shader.use();
		Shader::set_vec3("camera_position", camera.get_real_position());
		Shader::set_vec3("light_position", light.position);
		Shader::set_sampler("my_texture", bitmaps["car_texture.png"], 1);
		Shader::set_sampler("my_other_texture", bitmaps["uv.png"], 3);
		Shader::set_mat4("position_transform", &t);
		Shader::set_bool("reflecting", true);

		if (current_model) current_model->draw();


		// draw the light (so we now where it is)
		al_identity_transform(&t);
		al_translate_transform_3d(&t, light.position.x, light.position.y, light.position.z);

		fresnel_shader.use();
		Shader::set_vec3("camera_position", camera.get_real_position());
		Shader::set_vec3("light_position", light.position);
		Shader::set_mat4("position_transform", &t);

		unit_sphere.draw();


		// restore the identity transform before the display gets flipped (and turn off shaders for good measure)
		al_identity_transform(&t);
		al_use_transform(&t);
		Shader::stop();
	}
};




class MyGUIScreen : public UIScreen
{
private:
	My3DProject *scene;
	UIToggleButton *button;
	UIListSpinner *model_choice_spinner;
	UIListSpinner *shader_choice_spinner;
	UIDial *dial;

	// state
	bool camera_spinning;

public:
	MyGUIScreen(Display *display, My3DProject *scene)
		: UIScreen(display)
		, scene(scene)
		, button(new UIToggleButton(this, 110, display->height()-70, 160, 50, "camera spin"))
		, model_choice_spinner(new UIListSpinner(this, 110, display->height()-120, 160, 30))
		, shader_choice_spinner(new UIListSpinner(this, 110, display->height()-120-60, 160, 30))
		, dial(new UIDial(this, 240, display->height()-70, 60))
		, camera_spinning(false)
	{
		dial->set_value(scene->model_scale);

		button->toggle();

		// create the model choices
		model_choice_spinner->add_item("allegro_flare_logo-02.obj");
		model_choice_spinner->add_item("car2-04.obj");
		//model_choice_spinner->add_item("male_head.obj");
		model_choice_spinner->add_item("unit_sphere4.obj");

		// create some shader choices
		shader_choice_spinner->add_item("multi");
		shader_choice_spinner->add_item("cubemap");
		shader_choice_spinner->add_item("fresnel");
		shader_choice_spinner->add_item("metalic");
	}
	void on_message(UIWidget *sender, std::string message) override
	{
		if (sender == button) camera_spinning = !camera_spinning;
		if (sender == dial) scene->model_scale = static_cast<UIDial *>(sender)->get_value() / 5.0;
		if (sender == model_choice_spinner) scene->set_model(model_choice_spinner->get_val());
		if (sender == shader_choice_spinner) scene->set_shader(shader_choice_spinner->get_val());
	}
	void on_timer() override
	{
		// spin the camera
		if (camera_spinning) scene->camera.spin += 1.0 / 60.0 * 0.1;
	}
};




int main(int argc, char **argv)
{
	Framework::initialize();
	Display *display = Framework::create_display(960, 600, ALLEGRO_OPENGL | ALLEGRO_PROGRAMMABLE_PIPELINE);
	My3DProject *proj = new My3DProject(display);
	MyGUIScreen *gui = new MyGUIScreen(display, proj);
	Framework::run_loop();
	return 0;
}




