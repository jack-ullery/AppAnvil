#ifndef GTKMM_EXAMPLE_HELLOWORLD_H
#define GTKMM_EXAMPLE_HELLOWORLD_H

#include <gtkmm/box.h>
#include <gtkmm/builder.h>
#include <gtkmm/button.h>
#include <gtkmm/label.h>
#include <gtkmm/scrolledwindow.h>
#include <string>
#include <vector>

class HelloWorld : public Gtk::ScrolledWindow
{
  public:
    HelloWorld();

  protected:
    //Signal handlers:
    void on_button_clicked();

    // GUI Builder to parse UI from xml file
    Glib::RefPtr<Gtk::Builder> builder;

    //Member widgets:
    std::unique_ptr<Gtk::Label>   h_label;
    std::unique_ptr<Gtk::Button>  h_btn;
    std::unique_ptr<Gtk::Box>     h_box;

    const std::vector<std::string> messages{"Hello!", "Привет", "Ha Lo", "Hej", "Hallo", "नमस्कार", "Hola", "Bonjour", "I'm Done", "Fin"};
    int message_index = 0;

  private:
    template <typename T_Widget>
    static std::unique_ptr<T_Widget> get_widget(const Glib::ustring name, const Glib::RefPtr<Gtk::Builder> &builder);
};

#endif // GTKMM_EXAMPLE_HELLOWORLD_H
