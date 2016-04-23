

#include <allegro_flare/allegro_flare.h>




class MyProject : public FGUIScreen
{
private:
	FGUIButton *button;
public:
	MyProject(Display *display) : FGUIScreen(display), button(NULL)
	{
		// create our button
		button = new FGUIButton(this, 200, 200, 140, 70, "Click Me!");
	}
	void on_message(FGUIWidget *sender, std::string message) override
	{
		if (sender == button) // click is automatic message to parent for FGUIButton
		{
			// animate the button around randomly when it gets clicked
			af::motion.cmove_to(&button->place.position.x, random_float(100, 600), 0.4);
			af::motion.cmove_to(&button->place.position.y, random_float(100, 500), 0.4);
			af::motion.cmove_to(&button->place.rotation, random_float(-0.4, 0.4), 0.4);
		}
	}
};



int main(int argc, char **argv)
{
	af::initialize();
	Display *display = af::create_display();
	MyProject *proj = new MyProject(display);
	af::run_loop();

	return 0;
}


