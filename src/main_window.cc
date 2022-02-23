#include "main_window.h"

#include <tuple>

MainWindow::MainWindow() : prof{new Profiles()}, proc{new Processes()}, logs{new Logs()}, console{new ConsoleThread(prof, proc, logs)}
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

  // Connect the profile tab to the `send_status_change` method
  sigc::slot<void(const std::string &, const std::string &, const std::string &)> change_fun =
      sigc::mem_fun(*this, &MainWindow::send_status_change);
  prof->set_status_change_signal_handler(change_fun);

  // Set some default properties for titlebar
  m_headerbar.set_custom_title(m_switcher);
  m_headerbar.set_title("AppAnvil");
  m_headerbar.set_subtitle("AppArmor GUI");
  m_headerbar.set_hexpand(true);
  m_headerbar.set_show_close_button(true);

  // Set the icon
  auto builder = Gtk::Builder::create_from_resource("/resources/icon.glade");
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