#ifndef SRC_MAIN_WINDOW_H
#define SRC_MAIN_WINDOW_H

#include "console_thread.h"
#include "tabs/about.h"
#include "tabs/controller/logs_controller.h"
#include "tabs/controller/processes_controller.h"
#include "tabs/controller/profiles_controller.h"
#include "tabs/file_chooser.h"
#include "tabs/model/status_column_record.h"
#include "tabs/view/logs.h"
#include "tabs/view/processes.h"
#include "tabs/view/profiles.h"

#include <gtkmm/applicationwindow.h>
#include <gtkmm/builder.h>
#include <gtkmm/headerbar.h>
#include <gtkmm/stack.h>
#include <gtkmm/stackswitcher.h>
#include <memory>

constexpr int DEFAULT_WINDOW_WIDTH  = 800;
constexpr int DEFAULT_WINDOW_HEIGHT = 500;

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
  typedef ProfilesController<Profiles, StatusColumnRecord> ProfilesControllerInstance;
  typedef ProcessesController<Processes, StatusColumnRecord> ProcessesControllerInstance;
  typedef LogsController<Logs, StatusColumnRecord> LogsControllerInstance;

  typedef ConsoleThread<ProfilesControllerInstance,
                        ProcessesControllerInstance,
                        LogsControllerInstance> ConsoleThreadInstance;
 
  // GUI Builder to parse UI from xml file
  Glib::RefPtr<Gtk::Builder> builder;

  // Member widgets:
  Gtk::Stack m_stack;
  Gtk::HeaderBar m_headerbar;
  Gtk::StackSwitcher m_switcher;

  // Controllers
  std::shared_ptr<ProfilesControllerInstance>  prof_control;
  std::shared_ptr<ProcessesControllerInstance> proc_control;
  std::shared_ptr<LogsControllerInstance>      logs_control;
  std::shared_ptr<FileChooser>                 file_chooser;
  std::shared_ptr<About>                       about;

  // Second thread for calling command line utilities
  std::shared_ptr<ConsoleThreadInstance> console;
};

#endif // MAIN_WINDOW_H
