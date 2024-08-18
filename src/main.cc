#include "main_window.h"
#include "tabs/view/apparmor_not_installed.h"
#include "threads/command_caller.h"

// NOLINTNEXTLINE(bugprone-suspicious-include)
#include "resource.autogen.c"

#include <gtkmm/application.h>

/**
 * @brief Globally registers the resources from resource.autogen.c
 *
 * @details
 * The Makefile automatically compiles the resources in (./resources) into resource.autogen.c using
 * glib-compile-resources
 */
void register_resource_bundle()
{
  Glib::RefPtr<Gio::Resource> resource_bundle = Glib::wrap(resources_get_resource());
  resource_bundle->register_global();
}

int main()
{
  auto app = Gtk::Application::create("com.github.jack-ullery");
  register_resource_bundle();

  // If AppArmor is not enabled, send an error message
  if(!CommandCaller::get_enabled())
  {
    auto dialog = AppArmorNotInstalled::get_dialog();
    return app->run(*dialog);
  }

  // Shows the window and returns when it is closed.
  MainWindow win;
  return app->run(win);
}
