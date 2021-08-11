#include "resource.autogen.c"

#include "main_window.h"
#include <gtkmm/application.h>

Glib::RefPtr<Gio::Resource> get_resource_bundle(){
  Glib::RefPtr<Gio::Resource> resource_bundle = Glib::wrap(resources_get_resource());
  resource_bundle->register_global();
  return resource_bundle;
}

int main()
{
  auto app = Gtk::Application::create("com.github.jack-ullery");
  auto resource_bundle = get_resource_bundle();

  // Shows the window and returns when it is closed.
  MainWindow win;
  return app->run(win);
}
