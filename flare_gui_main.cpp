

#include <flare_gui/flare_gui.h>




class MyProject : public FGUIScreen
{
public:
	MyProject(Display *display) : FGUIScreen(display)
	{
		FGUIButton *button = new FGUIButton(this, 200, 200, 100, 60, "Hello!");
	}
};



int main(int argc, char **argv)
{
	af::initialize();
	Display *display = af::create_display();
	MyProject *proj = new MyProject(display);
	af::run_loop();
}


