

#include <flare_gui/flare_gui.h>
#include <allegro5/allegro_opengl.h>

#include <allegro_flare/cubemap.h>



class Camera3
{
public:
	vec3d position;
	vec3d stepout;
	float spin;
	float tilt;
	Camera3()
		: position(0, 0, 0)
		, stepout(0, 0, 0)
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





class Entity
{
private:
	static Shader *standard_compound_shader;
	static Shader *__get_standard_compound_shader()
	{
		if (standard_compound_shader) return standard_compound_shader;

		std::cout << "Building Entity::standard_compound_shader." << std::endl;
		standard_compound_shader = new Shader("data/shaders/standard_compound_vertex.glsl", "data/shaders/standard_compound_fragment.glsl");
		if (!standard_compound_shader)
		{
			std::cout << "There was a problem creating the compound shader for Entity" << std::endl;
		}
		return standard_compound_shader;
	}

public:
  placement3d place;
  placement3d velocity;
  ModelNew *model;
  bool shader_applies_transform;

  // Eventually, there could be n shaders for each model
  // for example, one for each named object.  Initially,
  // however, we'll stick with one shader and texture set
  // per entity.
  Shader *shader;
  ALLEGRO_BITMAP *diffuse_texture;
  ALLEGRO_BITMAP *normal_texture;
  ALLEGRO_BITMAP *specular_texture;
  ALLEGRO_FLARE_CUBEMAP_TEXTURE *cube_map_A;
  ALLEGRO_FLARE_CUBEMAP_TEXTURE *cube_map_B;
  bool cube_map_reflecting;

  Entity()
	  : model(NULL)
	  , shader(__get_standard_compound_shader())
	  , shader_applies_transform(true)
	  , diffuse_texture(NULL)
	  , normal_texture(NULL)
	  , specular_texture(NULL)
	  , cube_map_A(NULL)
	  , cube_map_B(NULL)
	  , cube_map_reflecting(true)
  {}

  void draw(vec3d camera_position, vec3d light_position)
  {
		ALLEGRO_STATE previous_state;
		ALLEGRO_TRANSFORM transform;

		
		if (shader && shader_applies_transform)
		{
			// construct our entity's transform
			place.build_transform(&transform);

			// Now apply it to the shader
			shader->use();
			Shader::set_vec3("camera_position", camera_position);
			Shader::set_vec3("light_position", light_position);
			Shader::set_mat4("position_transform", &transform);

			Shader::set_bool("reflecting", cube_map_reflecting);

			Shader::set_sampler("diffuse_texture", diffuse_texture, 2);
			Shader::set_sampler("specular_texture", specular_texture, 3);
			Shader::set_sampler("normal_texture", normal_texture, 4);
			Shader::set_sampler_cube("cube_map_A", cube_map_A, 5);
			Shader::set_sampler_cube("cube_map_B", cube_map_B, 6);
		}
		else
		{
			// when not using the shader, we'll need to 
			// apply the transform directly here
			al_store_state(&previous_state, ALLEGRO_STATE_TRANSFORM);
			al_identity_transform(&transform);
			al_use_transform(&transform);
			place.start_transform();

			// also, we set the texture on this model directly
			if (diffuse_texture) model->set_texture(diffuse_texture);
		}



		// actually draw our model here
		if (model) model->draw();



		if (shader && shader_applies_transform)
		{
			Shader::stop();
		}
		else
		{
			place.restore_transform();
			al_restore_state(&previous_state);
		}
  }
};
Shader *Entity::standard_compound_shader = NULL;






class My3DProject : public Screen
{
public:
	// bins
	BitmapBin bitmaps;
	//ModelBin models;

	// assets
	Camera3 camera;
	//ModelNew construct;
	//ModelNew model;
	//ModelNew *current_model;
	//ModelNew unit_sphere;
	//Shader metalic_shader;
	//Shader fresnel_shader;
	Shader cubemap_shader;
	//Shader multi_shader;
	//Shader *current_shader;
	ALLEGRO_FLARE_CUBEMAP_TEXTURE *cube_map_A;
	ALLEGRO_FLARE_CUBEMAP_TEXTURE *cube_map_B;
	Light light;

	std::vector<Entity *> entities;
   Entity *skybox;

	My3DProject(Display *display)
		: Screen(display)
		, bitmaps()
		, camera()
		//, metalic_shader("data/shaders/metalic_vertex.glsl", "data/shaders/metalic_fragment.glsl")
		//, fresnel_shader("data/shaders/fresnel_vertex.glsl", "data/shaders/fresnel_fragment.glsl")
		, cubemap_shader("data/shaders/cube_vertex.glsl", "data/shaders/cube_fragment.glsl")
		//, multi_shader("data/shaders/multi_vertex.glsl", "data/shaders/multi_fragment.glsl")
		, light(4, 4, 3)
		, cube_map_A(NULL)
		, cube_map_B(NULL)
		, skybox(NULL)
	{
		camera.stepout = vec3d(0, 1.5, 8);
		camera.tilt = 0.3;

		cube_map_A = glsl_create_cubemap_from_vertical_strip("data/bitmaps/sky4.png");
		cube_map_B = glsl_create_cubemap_from_vertical_strip("data/bitmaps/sky5.png");

		skybox = new Entity();
		skybox->model = new ModelNew();
		skybox->model->load_obj_file("data/models/skybox-01.obj", 30.0/4.0);
		skybox->shader = &cubemap_shader;
		skybox->cube_map_A = cube_map_B;
		skybox->cube_map_B = cube_map_B;
		skybox->cube_map_reflecting = false;

		construct_scene();
	}


	void construct_scene()
	{
		Entity *entity = NULL;

		// add the Construct
		entity = new Entity();
		entity->shader = NULL;
		entity->model = new ModelNew();
			entity->model->load_obj_file("data/models/flat_stage-01.obj");
			entity->model->set_texture(bitmaps["uv.png"]);
		entities.push_back(entity);

		// add some nice models for us to look at
		ModelNew *a_nice_model = new ModelNew();
		a_nice_model->load_obj_file("data/models/allegro_flare_logo-03b.obj", 0.5);
		for (unsigned i=0; i<40; i++)
		{
			entity = new Entity();
			entity->model = a_nice_model;
			entity->cube_map_B = cube_map_B;
			entity->place.position = vec3d(random_float(-4, 4), random_float(0, 8), random_float(-4, 4));
			entity->place.rotation = vec3d(random_float(0, 1), random_float(0, 1), random_float(0, 1));
			entity->velocity.rotation = vec3d(random_float(0, 0.001), random_float(0, 0.001), random_float(0, 0.001));
			entities.push_back(entity);
			if ((i%2) == 0)
			{
				entity->shader = &cubemap_shader;
				entity->cube_map_A = cube_map_A;
				entity->cube_map_B = cube_map_B;
			}
		}
	}

	void setup_projection()
	{
		// setup the render settings
		al_set_render_state(ALLEGRO_DEPTH_TEST, 1);
		al_set_render_state(ALLEGRO_WRITE_MASK, ALLEGRO_MASK_DEPTH | ALLEGRO_MASK_RGBA);
		al_clear_depth_buffer(1);


		ALLEGRO_TRANSFORM t;

		camera.reverse_position_transform(&t);
	
		float aspect_ratio = (float)al_get_bitmap_height(backbuffer_sub_bitmap) / al_get_bitmap_width(backbuffer_sub_bitmap);
		al_perspective_transform(&t, -1, aspect_ratio, 1, 1, -aspect_ratio, 1000);

		//al_set_target_bitmap(backbuffer_sub_bitmap); << I don't think this is necessairy, it also occours just prior to this function
		al_use_projection_transform(&t);
	}
	void primary_timer_func() override
	{
		//
		// UPDATE
		//

		for (unsigned i=0; i<entities.size(); i++)
		{
			entities[i]->place.position += entities[i]->velocity.position;
			entities[i]->place.rotation += entities[i]->velocity.rotation;
		}



		//
		// DRAW
		//

		// the functionality of this line might eventually be embedded in the framework
		setup_projection();

		// draw the entities in our scene
		vec3d camera_position = camera.get_real_position();
		vec3d light_position = light.position;

		// draw the skybox (and clear the depth buffer)
		skybox->place.position = camera_position;
		skybox->draw(0, 0);
		al_clear_depth_buffer(1);

		for (unsigned i=0; i<entities.size(); i++)
		{
			entities[i]->draw(camera_position, light_position);
		}



		// draw the Construct (the environment box)
/*
		glEnable(GL_CULL_FACE); // requiring opengl should eventually be fazed out
		construct.draw();
		glDisable(GL_CULL_FACE);
*/

	}
};




class MyGUIScreen : public FGUIScreen
{
private:
	My3DProject *scene;
	FGUIToggleButton *button;
	FGUIListSpinner *shader_choice_spinner;

	// state
	bool camera_spinning;

public:
	MyGUIScreen(Display *display, My3DProject *scene)
		: FGUIScreen(display)
		, scene(scene)
		, button(new FGUIToggleButton(this, 110, display->height()-70, 160, 50, "camera spin"))
		, camera_spinning(false)
	{
		button->toggle();
	}
	void on_message(FGUIWidget *sender, std::string message) override
	{
		if (sender == button) camera_spinning = !camera_spinning;
	}
	void on_timer() override
	{
		// spin the camera
		if (camera_spinning) scene->camera.spin += 1.0 / 60.0 * 0.1;
	}
};




int main(int argc, char **argv)
{
	af::initialize();
	Display *display = af::create_display(960*2, 600*2, ALLEGRO_OPENGL | ALLEGRO_PROGRAMMABLE_PIPELINE);
	My3DProject *proj = new My3DProject(display);
	MyGUIScreen *gui = new MyGUIScreen(display, proj);
	af::run_loop();
	return 0;
}




