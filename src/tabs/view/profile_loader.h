#ifndef TABS_PROFILELOADER_H
#define TABS_PROFILELOADER_H

#include <gtkmm/builder.h>
#include <gtkmm/enums.h>
#include <gtkmm/filechooser.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/searchentry.h>
#include <memory>
#include <string>
#include <vector>

class ProfileLoader : public Gtk::ScrolledWindow
{
  public:
    ProfileLoader();

    void set_label_text(const std::string &str);
    Glib::RefPtr<Gio::File> get_file();

    void set_l_button_signal_handler(const Glib::SignalProxyProperty::SlotType &func);

  private:
    // GUI Builder to parse UI from xml file
    Glib::RefPtr<Gtk::Builder> builder;

    // Widgets on the tab   
    std::unique_ptr<Gtk::Box> l_box;
    std::unique_ptr<Gtk::FileChooser> l_filechooser_button;
    std::unique_ptr<Gtk::Label> l_confirm_label;
    std::unique_ptr<Gtk::Button> l_confirm_button;

    template <typename T_Widget>
    static std::unique_ptr<T_Widget> get_widget(Glib::ustring name, const Glib::RefPtr<Gtk::Builder>& builder);
};

#endif // TABS_PROFILES_H
