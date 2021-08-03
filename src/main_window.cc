#include "main_window.h"
#include <iostream>

#include <gtkmm/button.h>
#include <gtkmm/label.h>

MainWindow::MainWindow()
: builder{Gtk::Builder::create_from_file("./resources/main_window.glade")},
  hw(new HelloWorld())
{
  // Get the main container and add it to the current window
  builder->get_widget<Gtk::Box>("m_box", m_container);
  builder->get_widget<Gtk::Stack>("m_stack", m_stack);
  this->add(*m_container);

  // Add the Hello World page to the stack
  m_stack->add(*hw,"page3", "Hello World");

  // Set some default settings for the window
  this->set_title("Skeleton GTK Demo");
  this->set_icon_from_file("./resources/icon.svg");
  this->set_default_size(400, 400);

  // Show all children
  this->show_all();
}

MainWindow::~MainWindow()
{
  std::cout << "Closing Main Window!" << std::endl;
  // Delete all tabs
  hw.reset();
  // Delete all the widgets
  delete m_stack;
  delete m_container;
}
