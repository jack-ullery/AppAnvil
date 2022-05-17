#include "file_chooser.h"

#include <iostream>
#include <string>
#include <vector>

template <typename T_Widget>
std::unique_ptr<T_Widget> FileChooser::get_widget(Glib::ustring name, const Glib::RefPtr<Gtk::Builder> &builder){
  T_Widget *raw_addr = nullptr;
  builder->get_widget<T_Widget>(name, raw_addr);
  return std::unique_ptr<T_Widget>(raw_addr);
}

FileChooser::FileChooser()
: builder{Gtk::Builder::create_from_file("./resources/load_profile.glade")},
  l_box{FileChooser::get_widget<Gtk::Box>("l_box", builder)},
  l_filechooser_button{FileChooser::get_widget<Gtk::FileChooser>("l_filechooser_button", builder)},
  l_confirm_label{FileChooser::get_widget<Gtk::Label>("l_confirm_label", builder)},
  l_confirm_button{FileChooser::get_widget<Gtk::Button>("l_confirm_button", builder)}
{
  l_box->set_hexpand();
  l_box->set_vexpand();
  
  this->add(*l_box);
}

void FileChooser::set_l_button_signal_handler(const Glib::SignalProxyProperty::SlotType &func){
  l_confirm_button->signal_clicked().connect(func, true);
}

void FileChooser::set_label_text(const std::string &str){
  l_confirm_label->set_text(str);
}

Glib::RefPtr<Gio::File> FileChooser::get_file(){
  return l_filechooser_button->get_file();
}