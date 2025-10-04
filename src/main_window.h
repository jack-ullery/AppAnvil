#ifndef SRC_MAIN_WINDOW_H
#define SRC_MAIN_WINDOW_H

#include "console_thread.h"
#include "tabs/controller/logs_controller.h"
#include "tabs/controller/processes_controller.h"
#include "tabs/controller/profiles_controller.h"
#include "tabs/model/database.h"
#include "tabs/model/process_adapter.h"
#include "tabs/model/profile_adapter.h"
#include "tabs/model/status_column_record.h"
#include "tabs/view/help.h"
#include "tabs/view/logs.h"
#include "tabs/view/processes.h"
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

// Duration in milliseconds
constexpr int DEFAULT_TRANSITION_DURATION = 150;

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
  // Signal handlers:

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

  /**
   * @brief Calls the show_reauthenticate_prompt() for each tab
   * 
   * @param should_show_reauth 
   */
  void show_reauth(bool should_show_reauth);
  inline std::function<void(bool)> get_show_reauth_func();

private:
  // A set of Typedeffed classes, to handle dependency injection
  // This lowers the amount of repeated <..> symbols
  typedef ProfilesController<Profiles, Database, ProfileAdapter<Database>> ProfilesControllerInstance;
  typedef ProcessesController<Processes, Database, ProcessAdapter<Database, StatusColumnRecord>> ProcessesControllerInstance;
  typedef LogsController<Logs, Database, LogAdapter<Database, StatusColumnRecord>, LogRecord> LogsControllerInstance;

  // GUI Builder to parse UI from xml file
  Glib::RefPtr<Gtk::Builder> builder;

  // Member widgets:
  Gtk::Stack m_tab_stack;
  Gtk::HeaderBar m_headerbar;
  Gtk::StackSwitcher m_switcher;
  Gtk::ToggleButton m_search_button;

  // Custom ToggleButton, which opens popup containers
  Help help_toggle;

  // Data structure for sharing data related to AppArmor between tabs
  std::shared_ptr<Database> database;

  // Controllers
  std::shared_ptr<ProfilesControllerInstance> prof_control;
  std::shared_ptr<ProcessesControllerInstance> proc_control;
  std::shared_ptr<LogsControllerInstance> logs_control;

  // Second thread for calling command line utilities
  std::shared_ptr<ConsoleThread> console;
};

#endif // MAIN_WINDOW_H
