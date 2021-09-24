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
: builder{Gtk::Builder::create_from_resource("/resources/main_window.glade")},
  m_container{MainWindow::get_widget<Gtk::Box>("m_box", builder)},
  m_stack{MainWindow::get_widget<Gtk::Stack>("m_stack", builder)},
  prof{new Profiles()},
  proc{new Processes()},
  logs{new Logs()}
{
  // Get the main container and add it to the current window
  this->add(*m_container);

  // Add tabs to the stack pane
  m_stack->add(*prof, "prof", "Profiles");
  m_stack->add(*proc, "proc", "Processes");
  m_stack->add(*logs, "logs", "Logs");

  // Set some default settings for the window
  this->set_title("AppAnvil (pre-alpha release)");
  this->set_icon_from_file("./resources/icon.svg");
  this->set_default_size(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
  this->add_events(Gdk::EventMask::ENTER_NOTIFY_MASK);

  // Show all children
  this->show_all();
}

MainWindow::~MainWindow()
{
  std::cout << "Closing Main Window!" << std::endl;
}
