#ifndef SRC_TABS_DIALOG_WORLD
#define SRC_TABS_DIALOG_WORLD

#include "hello_world.h"

#include <gtkmm/box.h>
#include <gtkmm/builder.h>
#include <gtkmm/button.h>
#include <gtkmm/label.h>
#include <gtkmm/dialog.h>
#include <gtkmm/scrolledwindow.h>
#include <memory>
#include <string>
#include <vector>

constexpr int DEFAULT_POPUP_WIDTH  = 800;
constexpr int DEFAULT_POPUP_HEIGHT = 500;

class DialogWorld : public Gtk::ScrolledWindow
{

public:
  DialogWorld();
  virtual ~DialogWorld();

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

  std::unique_ptr<HelloWorld> hw;

  std::unique_ptr<Gtk::Dialog> m_diag;
};

#endif // SRC_TABS_DIALOG_WORLD
