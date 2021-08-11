#include "main_window.h"

#include <gtkmm/button.h>
#include <gtkmm/label.h>
#include <iostream>

template <typename T_Widget>
std::unique_ptr<T_Widget> MainWindow::get_widget(const Glib::ustring name, const Glib::RefPtr<Gtk::Builder> &builder){
  T_Widget *raw_addr = nullptr;
  builder->get_widget<T_Widget>(name, raw_addr);
  return std::unique_ptr<T_Widget>(raw_addr);
}

MainWindow::MainWindow()
: builder{Gtk::Builder::create_from_file("./resources/main_window.glade")},
  m_container{MainWindow::get_widget<Gtk::Box>("m_box", builder)},
  m_stack{MainWindow::get_widget<Gtk::Stack>("m_stack", builder)}
{
  // Get the main container and add it to the current window
  this->add(*m_container);

  // Set some default settings for the window
  this->set_title("Skeleton GTK Demo");
  this->set_icon_from_file("./resources/icon.svg");
  this->set_default_size(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);

  // Show all children
  this->show_all();
}

MainWindow::~MainWindow()
{
  std::cout << "Closing Main Window!" << std::endl;
}
