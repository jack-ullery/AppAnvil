#ifndef GTKMM_EXAMPLE_MAINWINDOW_H
#define GTKMM_EXAMPLE_MAINWINDOW_H

#include <gtkmm/applicationwindow.h>
#include <gtkmm/builder.h>
#include <gtkmm/box.h>
#include <gtkmm/stack.h>

#define DEFAULT_WINDOW_WIDTH  400
#define DEFAULT_WINDOW_HEIGHT 400

class MainWindow : public Gtk::ApplicationWindow
{
  public:
    MainWindow();
    ~MainWindow() override;

  protected:
    //Signal handlers:
    void on_button_clicked();

    // Glib Resource Bundle that contains all the images and .glade files 
    Glib::RefPtr<Gio::Resource> resource_bundle;

    // GUI Builder to parse UI from xml file
    Glib::RefPtr<Gtk::Builder> builder;

    //Member widgets:
    Gtk::Box *m_container;
    Gtk::Stack *m_stack;

  private:
    static Glib::RefPtr<Gio::Resource> get_resource_bundle();
    
    template <typename T_Widget>
    static T_Widget* get_widget(const Glib::ustring name, const Glib::RefPtr<Gtk::Builder> &builder);
};

#endif // GTKMM_EXAMPLE_MAINWINDOW_H
