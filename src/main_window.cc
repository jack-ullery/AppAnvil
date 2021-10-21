#include "main_window.h"

#include <gtkmm/button.h>
#include <gtkmm/label.h>
#include <iostream>

MainWindow::MainWindow()
: prof{new Profiles()},
  proc{new Processes()},
  logs{new Logs()}
{
  // Add tabs to the stack pane
  m_stack.add(*prof, "prof", "Profiles");
  m_stack.add(*proc, "proc", "Processes");
  m_stack.add(*logs, "logs", "Logs");

  // Attach the stack to the stack switcher
  m_switcher.set_stack(m_stack);
  // m_headerbar.pack_end(m_switcher);

  // Set some default properties for titlebar
  m_headerbar.set_custom_title(m_switcher);
  m_headerbar.set_title("AppAnvil");
  m_headerbar.set_subtitle("AppArmor GUI");
  m_headerbar.set_hexpand(true);
  m_headerbar.set_show_close_button(true);


  // Set some default settings for the window
  this->set_icon_from_file("./resources/icon.svg");
  this->set_default_size(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
  this->add_events(Gdk::EventMask::ENTER_NOTIFY_MASK);

  // Add and show all children
  this->set_titlebar(m_headerbar);
  this->add(m_stack);
  this->show_all();
}