


#include <allegro_flare/allegro_flare.h>



class MyGUIMain : public UIScreen
{
private:
   UIButton *button;

public:
   MyGUIMain(Display *display)
      : UIScreen(display)
      , button(new UIButton(this, 200, 200, 140, 70, "Click Me!"))
   {}

   void on_message(UIWidget *sender, std::string message) override
   {
      if (sender == button) // click is automatic message to parent for UIButton
      {
         // animate the button around randomly when it gets clicked
         Framework::motion().cmove_to(&button->place.position.x, random_float(100, 600), 0.4);
         Framework::motion().cmove_to(&button->place.position.y, random_float(100, 500), 0.4);
         Framework::motion().cmove_to(&button->place.rotation, random_float(-0.4, 0.4), 0.4);
      }
   }
};



int main(int argc, char **argv)
{
   Framework::initialize();
   Display *display = Framework::create_display();
   MyGUIMain *proj = new MyGUIMain(display);
   Framework::run_loop();
   return 0;
}



