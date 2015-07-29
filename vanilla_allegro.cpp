


#include <allegro5/allegro.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_primitives.h>




int main(int argc, char* argv[])
{
	al_init();
	al_init_primitives_addon();

	ALLEGRO_DISPLAY *display = al_create_display(960, 600);

	al_draw_filled_circle(400, 300, 120, al_color_name("orange"));

	al_flip_display();

	al_rest(2);
}



