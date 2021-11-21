#include "main_window.h"

#include <gtkmm/button.h>
#include <gtkmm/label.h>
#include <iostream>

MainWindow::MainWindow()
: prof{new Profiles()},
  proc{new Processes()},
  logs{new Logs()},
  file_chooser{new FileChooser()},
  console{new ConsoleThread(prof, proc, logs, file_chooser)}
{
  // Add tabs to the stack pane
  m_stack.add(*prof, "prof", "Profiles");
  m_stack.add(*proc, "proc", "Processes");
  m_stack.add(*logs, "logs", "Logs");
  m_stack.add(*file_chooser, "file_chooser", "Load Profile");

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
    console->set_state(PROFILE);
    console->send_refresh_message();
    // prof->refresh();
  } else if(visible_child == "proc"){
    console->set_state(PROCESS);
    console->send_refresh_message();
    // proc->refresh();    
  } else if(visible_child == "logs"){
    console->set_state(LOGS);
    console->send_refresh_message();
    // logs->refresh();
  } else if(visible_child == "file_chooser"){
    console->set_state(FILECHOOSER);
    console->send_refresh_message();
    // logs->refresh();
  }

  return false;
}