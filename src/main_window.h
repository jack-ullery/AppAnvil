#ifndef GTKMM_EXAMPLE_MAINWINDOW_H
#define GTKMM_EXAMPLE_MAINWINDOW_H

#include "tabs/hello_world.h"
#include <gtkmm/applicationwindow.h>
#include <gtkmm/builder.h>
#include <gtkmm/box.h>
#include <gtkmm/stack.h>

class MainWindow : public Gtk::ApplicationWindow
{

public:
  MainWindow();
  ~MainWindow() override;

protected:
  //Signal handlers:
  void on_button_clicked();

  // GUI Builder to parse UI from xml file
  Glib::RefPtr<Gtk::Builder> builder;

  // Tabs
  std::shared_ptr<HelloWorld> hw;

  //Member widgets:
  Gtk::Box *m_container;
  Gtk::Stack *m_stack;
};

#endif // GTKMM_EXAMPLE_MAINWINDOW_H
