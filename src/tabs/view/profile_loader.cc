#include "profile_loader.h"
#include "common.h"

#include <iostream>
#include <string>
#include <vector>

ProfileLoader::ProfileLoader()
  : builder{ Gtk::Builder::create_from_resource("/resources/load_profile.glade") },
    l_box{ Common::get_widget<Gtk::Box>("l_box", builder) },
    l_filechooser_button{ Common::get_widget<Gtk::FileChooser>("l_filechooser_button", builder) },
    l_confirm_label{ Common::get_widget<Gtk::Label>("l_confirm_label", builder) },
    l_confirm_button{ Common::get_widget<Gtk::Button>("l_confirm_button", builder) }
{
  l_box->set_hexpand();
  l_box->set_vexpand();

  this->add(*l_box);
}

void ProfileLoader::set_l_button_signal_handler(const Glib::SignalProxyProperty::SlotType &func)
{
  l_confirm_button->signal_clicked().connect(func, true);
}

void ProfileLoader::set_label_text(const std::string &str)
{
  l_confirm_label->set_text(str);
}

Glib::RefPtr<Gio::File> ProfileLoader::get_file()
{
  return l_filechooser_button->get_file();
}