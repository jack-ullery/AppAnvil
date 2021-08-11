#include "main_window.h"
#include <gtkmm/application.h>

int main()
{
  auto app = Gtk::Application::create("com.github.jack-ullery");

  // Shows the window and returns when it is closed.
  MainWindow win;
  return app->run(win);
}
