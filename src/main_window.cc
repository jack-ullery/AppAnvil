#include "main_window.h"
#include "tabs/file_chooser.h"

#include <bits/types/FILE.h>
#include <tuple>
#include <iostream>

MainWindow::MainWindow()
    : prof_control{new ProfilesControllerInstance()}, 
      proc_control{new ProcessesControllerInstance()}, 
      logs_control{new LogsControllerInstance()}, 
      file_chooser{new FileChooser()}, 
      about{new About()}, 
      console{new ConsoleThreadInstance(prof_control, proc_control, logs_control)}
{
  // Add tabs to the stack pane
  m_stack.add(*(prof_control->get_tab()), "prof", "Profiles");
  m_stack.add(*(proc_control->get_tab()), "proc", "Processes");
  m_stack.add(*(logs_control->get_tab()), "logs", "Logs");
  m_stack.add(*file_chooser, "file_chooser", "Load Profile");
  m_stack.add(*about, "about", "About Me");

  // Attach the stack to the stack switcher
  m_switcher.set_stack(m_stack);

  // Connect the stackswitcher to the 'on_switch' method
  auto focus = sigc::mem_fun(*this, &MainWindow::on_switch);
  m_switcher.signal_event().connect(focus, true);
  on_switch(NULL);

  // Connect the profile tab to the `send_status_change` method
  sigc::slot<void(const std::string &, const std::string &, const std::string &)> change_fun =
      sigc::mem_fun(*this, &MainWindow::send_status_change);
  prof_control->get_tab()->set_status_change_signal_handler(change_fun);

  // Set some default properties for titlebar
  m_headerbar.set_custom_title(m_switcher);
  m_headerbar.set_title("AppAnvil");
  m_headerbar.set_subtitle("AppArmor GUI");
  m_headerbar.set_hexpand(true);
  m_headerbar.set_show_close_button(true);

  // Set the icon
  auto builder         = Gtk::Builder::create_from_resource("/resources/icon.glade");
  Gtk::Image *icon_ptr = nullptr;
  builder->get_widget<Gtk::Image>("icon", icon_ptr);
  this->set_icon(icon_ptr->get_pixbuf());

  // Set some default settings for the window
  this->set_default_size(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
  this->add_events(Gdk::EventMask::ENTER_NOTIFY_MASK);

  // Add and show all children
  this->set_titlebar(m_headerbar);
  this->add(m_stack);
  this->show_all();
}

void MainWindow::send_status_change(const std::string &profile, const std::string &old_status, const std::string &new_status)
{
  console->send_change_profile_status_message(profile, old_status, new_status);
}

bool MainWindow::on_switch(GdkEvent *event)
{
  std::ignore = event;
  
  //make sure to clear the success label on the load tab 
  file_chooser->clearLabel();

  std::string visible_child = m_stack.get_visible_child_name();

  if(visible_child == "prof") {
    console->send_refresh_message(PROFILE);
  } else if(visible_child == "proc") {
    console->send_refresh_message(PROCESS);
  } else if(visible_child == "logs") {
    console->send_refresh_message(LOGS);
  }

  return false;
}
