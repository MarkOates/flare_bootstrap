

#include <flare_gui/flare_gui.h>




class MyProject : public FGUIScreen
{
public:
	MyProject(Display *display) : FGUIScreen(display) {}
	void primary_timer_func() override
	{
		al_draw_rectangle(200, 200, 340, 280, color::red, 6.0);
	}
};



int main(int argc, char **argv)
{
	af::initialize();
	Display *display = af::create_display();
	MyProject *proj = new MyProject(display);
	af::run_loop();
}


