#ifndef SRC_MAIN_WINDOW_H
#define SRC_MAIN_WINDOW_H

#include "console_thread.h"
#include "tabs/controller/logs_controller.h"
#include "tabs/controller/processes_controller.h"
#include "tabs/controller/profile_loader_controller.h"
#include "tabs/controller/profiles_controller.h"
#include "tabs/model/database.h"
#include "tabs/model/process_adapter.h"
#include "tabs/model/profile_adapter.h"
#include "tabs/model/status_column_record.h"
#include "tabs/view/help.h"
#include "tabs/view/logs.h"
#include "tabs/view/processes.h"
#include "tabs/view/profile_loader.h"
#include "tabs/view/profiles.h"

#include <gtkmm/applicationwindow.h>
#include <gtkmm/builder.h>
#include <gtkmm/headerbar.h>
#include <gtkmm/stack.h>
#include <gtkmm/stackswitcher.h>
#include <memory>

// Using the golden ratio
constexpr int DEFAULT_WINDOW_WIDTH  = 1200;
constexpr int DEFAULT_WINDOW_HEIGHT = 742;

class MainWindow : public Gtk::ApplicationWindow
{
public:
  /**
   * @brief Constructor to build a Gtk::ApplicationWindow.
   *
   * @details
   * Constructor to build a Gtk::ApplicationWindow that initializes and show the entire application.
   * The MainWindow contains a stack that switches multiple different "tabs".
   */
  MainWindow();

protected:
  //Signal handlers:

  /**
   * @brief Untoggles the button that controls the 'Help' page.
   * 
   * @details
   * This function is used as a signal handler in the 'Help' page
   */
  void untoggle_help();

  /**
   * @brief Makes the 'Help' page visible whenever toggled.
   * 
   * @details
   * Hides the stack and stack switcher, when showing the 'Help' page
   */
  void on_help_toggle();

  /**
   * @brief Makes the searchbar visible, on each tab, whenever toggled.
   */
  void on_search_toggle();

  /**
   * @brief Calls refresh() on the visible tab when the stackswitcher is pressed.
   *
   * @details
   * We must have 'GDKEvent*' as a parameter so that it can be the signal handler for m_switcher.signal_event().
   * However, we never use this parameter.
   */
  bool on_switch(GdkEvent *event);

  void send_status_change(const std::string &profile, const std::string &old_status, const std::string &new_status);

private:
  // A set of Typedeffed classes, to handle dependency injection
  // This lowers the amount of repeated <..> symbols
  typedef ProfilesController<Profiles, StatusColumnRecord, Database, ProfileAdapter<Database>> ProfilesControllerInstance;
  typedef ProcessesController<Processes, StatusColumnRecord, Database, ProcessAdapter<Database>> ProcessesControllerInstance;
  typedef LogsController<Logs, Database, LogAdapter<Database>> LogsControllerInstance;
  typedef ProfileLoaderController<ProfileLoader> ProfileLoaderControllerInstance;

  typedef ConsoleThread<ProfilesControllerInstance,
                        ProcessesControllerInstance,
                        LogsControllerInstance> ConsoleThreadInstance;
 
  // GUI Builder to parse UI from xml file
  Glib::RefPtr<Gtk::Builder> builder;

  // Member widgets:
  Gtk::Stack m_top_stack;
  Gtk::Stack m_tab_stack;
  Gtk::HeaderBar m_headerbar;
  Gtk::StackSwitcher m_switcher;
  Gtk::ToggleButton m_help_button;
  Gtk::ToggleButton m_search_button;

  // Data structure for sharing data related to AppArmor between tabs
  std::shared_ptr<Database> database;

  // Controllers
  std::shared_ptr<ProfilesControllerInstance>    prof_control;
  std::shared_ptr<ProcessesControllerInstance>   proc_control;
  std::shared_ptr<LogsControllerInstance>        logs_control;
  std::shared_ptr<ProfileLoaderControllerInstance> profile_loader_control;

  // Tab (Without Controller)
  std::shared_ptr<Help> help;

  // Second thread for calling command line utilities
  std::shared_ptr<ConsoleThreadInstance> console;

  // Controls whether a the 'Search' button is visible
  void handle_search_button_visiblity();
};

#endif // MAIN_WINDOW_H
