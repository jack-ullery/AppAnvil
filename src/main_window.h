#ifndef SRC_MAIN_WINDOW_H
#define SRC_MAIN_WINDOW_H

#include "tabs/logs.h"
#include "tabs/processes.h"
#include "tabs/profiles.h"
#include "tabs/status.h"

#include <gtkmm/applicationwindow.h>
#include <gtkmm/box.h>
#include <gtkmm/builder.h>
#include <gtkmm/stack.h>
#include <memory>

constexpr int DEFAULT_WINDOW_WIDTH  = 800;
constexpr int DEFAULT_WINDOW_HEIGHT = 500;

class MainWindow : public Gtk::ApplicationWindow
{
  public:
    MainWindow();

  protected:
    //Signal handlers:
    void on_button_clicked();

  private:
    // GUI Builder to parse UI from xml file
    Glib::RefPtr<Gtk::Builder> builder;

    // Member widgets:
    std::unique_ptr<Gtk::Box>   m_container;
    std::unique_ptr<Gtk::Stack> m_stack;

    // Tabs
    std::unique_ptr<Profiles> prof;
    std::unique_ptr<Processes> proc;
    std::unique_ptr<Logs> logs;

    // Methods
    template <typename T_Widget>
    static std::unique_ptr<T_Widget> get_widget(Glib::ustring name, const Glib::RefPtr<Gtk::Builder> &builder);
};

#endif // MAIN_WINDOW_H
