

#include <allegro_flare/allegro_flare.h>




class MyProject : public Screen
{
public:
	MyProject(Display *display) : Screen(display) {}
	void primary_timer_func() override
	{
		al_draw_rectangle(200, 200, 340, 280, color::red, 6.0);
	}
};



int main(int argc, char **argv)
{
	Framework::initialize();
	Display *display = Framework::create_display();
	MyProject *proj = new MyProject(display);
	Framework::run_loop();
	return 0;
}


