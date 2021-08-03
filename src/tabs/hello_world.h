#ifndef GTKMM_EXAMPLE_HELLOWORLD_H
#define GTKMM_EXAMPLE_HELLOWORLD_H

#include <gtkmm/box.h>
#include <gtkmm/builder.h>
#include <gtkmm/button.h>
#include <gtkmm/label.h>
#include <gtkmm/scrolledwindow.h>
#include <string>
#include <vector>

class HelloWorld : public Gtk::ScrolledWindow
{

public:
  HelloWorld();
  virtual ~HelloWorld();

protected:
  //Signal handlers:
  void on_button_clicked();

  // GUI Builder to parse UI from xml file
  Glib::RefPtr<Gtk::Builder> builder;

  //Member widgets:
  Gtk::Label *h_label;
  Gtk::Button *h_btn;
  Gtk::Box *h_box;

  const std::vector<std::string> messages{"Hello!", "Привет", "Ha Lo", "Hej", "Hallo", "नमस्कार", "Hola", "Bonjour", "I'm Done", "Fin"};
  int message_index = 0;
};

#endif // GTKMM_EXAMPLE_HELLOWORLD_H
