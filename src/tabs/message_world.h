#ifndef SRC_TABS_MESSAGE_WORLD
#define SRC_TABS_MESSAGE_WORLD

#include <gtkmm/box.h>
#include <gtkmm/builder.h>
#include <gtkmm/button.h>
#include <gtkmm/label.h>
#include <gtkmm/messagedialog.h>
#include <gtkmm/scrolledwindow.h>
#include <memory>
#include <string>
#include <vector>

class MessageWorld : public Gtk::ScrolledWindow
{

public:
  MessageWorld();
  virtual ~MessageWorld();

protected:
  //Signal handlers:
  void on_button_clicked();

  // GUI Builder to parse UI from xml file
  Glib::RefPtr<Gtk::Builder> builder;

  //Member widgets:
  // This code was made without using smart pointers which is a bad practice
  // This branch is just to showcase an example of Gtk::Dialog
  Gtk::Button *h_btn;
  Gtk::Box *h_box;

  // The Dialog object we will be using
  std::unique_ptr<Gtk::MessageDialog> m_diag;

  const std::vector<std::string> messages{"Hello!", "Привет", "Ha Lo", "Hej", "Hallo", "नमस्कार", "Hola", "Bonjour", "I'm Done", "Fin"};
  int message_index = 0;
};

#endif // SRC_TABS_MESSAGE_WORLD
