#include "resource.autogen.c"

#include <giomm/resource.h>
#include <gtest/gtest.h>
#include <gtkmm/application.h>
#include <gtkmm/applicationwindow.h>

// This bundles and registers all the resources from the "resources" folder for use with the unit tests.
void register_resource_bundle()
{
  Glib::RefPtr<Gio::Resource> resource_bundle = Glib::wrap(resources_get_resource());
  resource_bundle->register_global();
}

/**
 * @details
 * This class runs the unit-tests for us from a GTK Application Window.
 * This tricks GTKmm into thinking our unit tests are a part of a GUI, which
 * allows us to create and test classes that rely on GTK components.
 */
class FakeWindow : public Gtk::ApplicationWindow
{
public:
  FakeWindow(int argc, char **argv)
  {
    // This runs all the tests
    testing::InitGoogleTest(&argc, argv);
    return_value = RUN_ALL_TESTS();
  }

  // This returns the value of the tests
  int get_value() // NOLINT(readability-make-member-function-const)
  {
    return return_value;
  }

private:
  int return_value;
};

int main(int argc, char **argv)
{
  // Create a GTK application to run our tests in
  auto app = Gtk::Application::create("github.com.jack-ullery.appanvil.test");
  // Register the Glade resources
  register_resource_bundle();
  // Create a GTK window that runs the unit tests for us
  // The constructor of this window runs all of the unit tests and saves the result
  FakeWindow win(argc, argv);
  // Return the results of the unit tests
  return win.get_value();
}