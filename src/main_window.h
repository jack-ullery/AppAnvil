#ifndef SRC_MAIN_WINDOW_H
#define SRC_MAIN_WINDOW_H

#include "console_thread.h"
#include "tabs/file_chooser.h"
#include "tabs/logs.h"
#include "tabs/parser.h"
#include "tabs/processes.h"
#include "tabs/profiles.h"

#include "tabs/status.h"
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
    bool on_switch(GdkEvent* event);

    void send_status_change(const std::string& profile, const std::string& old_status, const std::string& new_status);

  private:
    // GUI Builder to parse UI from xml file
    Glib::RefPtr<Gtk::Builder> builder;

    // Member widgets:
    Gtk::Stack m_stack;
    Gtk::HeaderBar m_headerbar;
    Gtk::StackSwitcher m_switcher;

    // Tabs
    std::shared_ptr<Profiles> prof;
    std::shared_ptr<Processes> proc;
    std::shared_ptr<Logs> logs;
    std::shared_ptr<FileChooser> file_chooser;

    // Second thread for calling command line utilities
    std::shared_ptr<ConsoleThread> console;
};

#endif // MAIN_WINDOW_H
