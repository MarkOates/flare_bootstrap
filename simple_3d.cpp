


#include <allegro_flare/allegro_flare.h>




class SimpleEntity
{
public:
   placement3d place;
   placement3d velocity;
   Model3D *model;
   ALLEGRO_BITMAP *texture;

   SimpleEntity(Model3D *model, ALLEGRO_BITMAP *texture=NULL)
      : place()
      , velocity()
      , model(model)
      , texture(texture)
   {
      velocity.align = vec3d(0, 0, 0);
      velocity.scale = vec3d(0, 0, 0);
   }

   void draw()
   {
      if (!model) return;
      place.start_transform();
      model->set_texture(texture);
      model->draw();
      place.restore_transform();
   }
};




class Project : public Screen
{
public:
   std::vector<SimpleEntity *> entities;
   SimpleEntity camera;

   Project(Display *display)
      : Screen(display)
      , entities()
      , camera(NULL)
   {
      // pull the camera back from the center of the scene
      camera.place.anchor = vec3d(0, 0, 5);
      load_scene();
   }

   void load_scene()
   {
      SimpleEntity *e = new SimpleEntity(Framework::model("coin_ring-01.obj"));
      entities.push_back(e);

      for (unsigned i=0; i<10; i++)
      {
         SimpleEntity *e = new SimpleEntity(Framework::model("coin_ring-01.obj"));
         e->texture = Framework::bitmap("uv.png");
         e->place.position.x = random_float(-2, 2);
         e->place.position.y = random_float(-2, 2);
         e->place.position.z = random_float(-2, 2);
         e->velocity.rotation.y = 0.005;
         entities.push_back(e);
      }
   }

   void update_scene()
   {
      for (auto e : entities) e->place += e->velocity;

      camera.place.rotation.y += 0.001;
   }

   void draw_scene()
   {
      // setup the render settings
      al_set_render_state(ALLEGRO_DEPTH_TEST, 1);
      al_set_render_state(ALLEGRO_WRITE_MASK, ALLEGRO_MASK_DEPTH | ALLEGRO_MASK_RGBA);
      al_clear_depth_buffer(1);

      ALLEGRO_TRANSFORM t;
      camera.place.build_reverse_transform(&t);
      set_projection(backbuffer_sub_bitmap, &t);

      // draw our entities
      for (auto e : entities) e->draw();
   }

   void primary_timer_func() override
   {
      update_scene();
      draw_scene();
   }

private:
   void set_projection(ALLEGRO_BITMAP *bitmap, ALLEGRO_TRANSFORM *t)
   {
      float aspect_ratio = (float)al_get_bitmap_height(bitmap) / al_get_bitmap_width(bitmap);
      al_perspective_transform(t, -1, aspect_ratio, 1, 1, -aspect_ratio, 100);
      al_use_projection_transform(t);
   }
};




int main(int argc, char **argv)
{
   Framework::initialize();
   Display *display = Framework::create_display(800, 600, ALLEGRO_OPENGL | ALLEGRO_PROGRAMMABLE_PIPELINE);
   Project *project = new Project(display);
   Framework::run_loop();
   return 0;
}



