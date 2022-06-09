#include "main_window.h"

#include <gtkmm/button.h>
#include <gtkmm/togglebutton.h>
#include <tuple>

MainWindow::MainWindow()
    : database{new Database()},
      prof_control{new ProfilesControllerInstance(database)},
      proc_control{new ProcessesControllerInstance(database)},
      logs_control{new LogsControllerInstance(database)},
      profile_loader_control{new ProfileLoaderControllerInstance()},
      help{new Help()},
      console{new ConsoleThreadInstance(prof_control, proc_control, logs_control)}
{
  // Add tabs to the stack pane
  m_tab_stack.add(*(prof_control->get_tab()), "prof", "Profiles");
  m_tab_stack.add(*(proc_control->get_tab()), "proc", "Processes");
  m_tab_stack.add(*(logs_control->get_tab()), "logs", "Logs");
  m_tab_stack.add(*(profile_loader_control->get_tab()), "profile_loader", "Load Profile");

  // Attach the stack to the stack switcher
  m_switcher.set_stack(m_tab_stack);

  // Connect the stackswitcher to the 'on_switch' method
  auto focus = sigc::mem_fun(*this, &MainWindow::on_switch);
  m_switcher.signal_event().connect(focus, true);
  on_switch(NULL);

  // Connect the profile tab to the `send_status_change` method
  auto change_fun = sigc::mem_fun(*this, &MainWindow::send_status_change);
  prof_control->get_tab()->set_status_change_signal_handler(change_fun);

  // Configure settings related to the 'Help' button
  m_help_button.set_image_from_icon_name("dialog-question");

  auto help_toggle_fun = sigc::mem_fun(*this, &MainWindow::on_help_toggle);
  m_help_button.signal_toggled().connect(help_toggle_fun, true);

  // Simulate clicking the help toggle button (m_help_button), whenever somebody presses the bottom button on the help page
  auto activate_help_toggle_fun = sigc::mem_fun(*this, &MainWindow::untoggle_help);
  help->set_return_signal_handler(activate_help_toggle_fun);

  // Configure settings related to 'Search' button
  m_search_button.set_image_from_icon_name("edit-find-symbolic");

  auto search_togggle_fun = sigc::mem_fun(*this, &MainWindow::on_search_toggle);
  m_search_button.signal_toggled().connect(search_togggle_fun, true);

  // Add the main page and the help page to the top stack
  // This stack controls whether the 'Help' page is visible, or the main application 
  m_top_stack.add(m_tab_stack, "main_page");
  m_top_stack.add(*help, "help_page");

  // Set some default properties for titlebar
  m_headerbar.set_custom_title(m_switcher);
  m_headerbar.pack_end(m_help_button);
  m_headerbar.pack_end(m_search_button);

  m_headerbar.set_title("AppAnvil");
  m_headerbar.set_subtitle("GUI for AppArmor");
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
  this->add(m_top_stack);
  this->show_all();

  // Hide the side info in the Profiles Tab
  prof_control->get_tab()->hide_profile_info();
}

void MainWindow::send_status_change(const std::string &profile, const std::string &old_status, const std::string &new_status)
{
  console->send_change_profile_status_message(profile, old_status, new_status);
}

void MainWindow::on_help_toggle()
{
  bool is_active = m_help_button.get_active();

  if(is_active){
    m_switcher.hide();
    m_top_stack.set_visible_child("help_page");
    m_help_button.set_label("Return to application");
    m_help_button.set_always_show_image(false);
  }
  else {
    m_switcher.show();
    m_top_stack.set_visible_child("main_page");
    m_help_button.set_label("");
    m_help_button.set_image_from_icon_name("dialog-question");
    m_help_button.set_always_show_image(true);
  }

  handle_search_button_visiblity();
}

void MainWindow::untoggle_help(){
  m_help_button.set_active(false);
}

void MainWindow::on_search_toggle()
{
  bool is_active = m_search_button.get_active();

  if(is_active){
    prof_control->get_tab()->show_searchbar();
    proc_control->get_tab()->show_searchbar();
    logs_control->get_tab()->show_searchbar();
    help->show_searchbar();
  }
  else {
    prof_control->get_tab()->hide_searchbar();
    proc_control->get_tab()->hide_searchbar();
    logs_control->get_tab()->hide_searchbar();
    help->hide_searchbar();
  }
}

bool MainWindow::on_switch(GdkEvent *event)
{
  std::ignore = event;
  
  // Make sure to clear the label on the load-profiles tab 
  profile_loader_control->clearLabel();

  std::string visible_child = m_tab_stack.get_visible_child_name();
  if(visible_child == "prof") {
    console->send_refresh_message(PROFILE);
  } else if(visible_child == "proc") {
    console->send_refresh_message(PROCESS);
  } else if(visible_child == "logs") {
    console->send_refresh_message(LOGS);
  }

  handle_search_button_visiblity();
  return false;
}

void MainWindow::handle_search_button_visiblity()
{
  bool help_is_active = m_help_button.get_active();
  std::string visible_child = m_tab_stack.get_visible_child_name();

  if(visible_child == "profile_loader" && !help_is_active) {
    m_search_button.hide();
  } else {
    m_search_button.show();
  }
}
