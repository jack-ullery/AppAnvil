#ifndef GTKMM_EXAMPLE_MAINWINDOW_H
#define GTKMM_EXAMPLE_MAINWINDOW_H

#include "random_box.h"
#include <gtkmm/applicationwindow.h>
#include <gtkmm/builder.h>
#include <gtkmm/box.h>
#include <gtkmm/stack.h>

class RandomWindow : public Gtk::ApplicationWindow
{

public:
  RandomWindow();
  ~RandomWindow() override;

protected:
  // GUI Builder to parse UI from xml file
  std::shared_ptr<RandomBox> rb;
};

#endif // GTKMM_EXAMPLE_MAINWINDOW_H
