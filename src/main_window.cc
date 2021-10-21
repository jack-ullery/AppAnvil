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

  // Connect the stackswitcher to the 'on_switch' method
  auto focus = sigc::mem_fun(*this, &MainWindow::on_switch);
  m_switcher.signal_event().connect(focus, true);
  on_switch(NULL);

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

// "__attribute__ ((unused))" is a GCC-specific way to suppress the [-Wunused-parameter] warning
// We must have 'direction' as a parameter so that it can be the signal handler for m_switcher.signal_event() 
bool MainWindow::on_switch(__attribute__ ((unused)) GdkEvent* direction){
  std::string visible_child  = m_stack.get_visible_child_name();

  if(visible_child == "prof"){
    prof->refresh();
  } else if(visible_child == "proc"){
    proc->refresh();    
  } else if(visible_child == "logs"){
    logs->refresh();
  }

  return false;
}