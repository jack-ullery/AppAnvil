#ifndef GTKMM_EXAMPLE_MAINWINDOW_H
#define GTKMM_EXAMPLE_MAINWINDOW_H

#include "tabs/hello_world.h"
#include "tabs/profiles.h"
#include "tabs/processes.h"
#include "tabs/status.h"

#include <gtkmm/applicationwindow.h>
#include <gtkmm/builder.h>
#include <gtkmm/box.h>
#include <gtkmm/stack.h>
#include <memory>

#define DEFAULT_WINDOW_WIDTH  800
#define DEFAULT_WINDOW_HEIGHT 500

class MainWindow : public Gtk::ApplicationWindow
{
  public:
    MainWindow();
    ~MainWindow() override;

  protected:
    //Signal handlers:
    void on_button_clicked();

    // GUI Builder to parse UI from xml file
    Glib::RefPtr<Gtk::Builder> builder;

    //Member widgets:
    std::unique_ptr<Gtk::Box>   m_container;
    std::unique_ptr<Gtk::Stack> m_stack;

    // Tabs
    std::unique_ptr<HelloWorld> hw;
    std::unique_ptr<Profiles> prof;
    std::unique_ptr<Processes> proc;

  private:
    template <typename T_Widget>
    static std::unique_ptr<T_Widget> get_widget(const Glib::ustring name, const Glib::RefPtr<Gtk::Builder> &builder);
};

#endif // GTKMM_EXAMPLE_MAINWINDOW_H
