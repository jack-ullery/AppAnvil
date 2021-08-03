#include "random_box_window.h"
#include "random_box.h"
#include <iostream>

#include <gtkmm/button.h>
#include <gtkmm/label.h>

RandomWindow::RandomWindow()
: rb(new RandomBox())
{
  // Get the main container and add it to the current window
  this->add(*rb);

  // Set some default settings for the window
  this->set_title("Not So Random Box!");
  this->set_icon_from_file("./resources/icon.svg");
  this->set_default_size(400, 400);

  // Show all children
  this->show_all();
}

RandomWindow::~RandomWindow(){}
