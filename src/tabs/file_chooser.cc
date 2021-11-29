#include "jsoncpp/json/json.h"
#include "file_chooser.h"
#include "../threads/command_caller.h"

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
  l_upper_text{FileChooser::get_widget<Gtk::TextView>("l_upper_text", builder)},
  l_mid_grid{FileChooser::get_widget<Gtk::Grid>("l_mid_grid", builder)},
  l_filechooser_button{FileChooser::get_widget<Gtk::Button>("l_filechooser_button", builder)},
  l_filechooser_label{FileChooser::get_widget<Gtk::Label>("l_filechooser_label", builder)},
  l_lower_text{FileChooser::get_widget<Gtk::TextView>("l_lower_text", builder)},
  l_lower_grid{FileChooser::get_widget<Gtk::Grid>("l_lower_grid", builder)},
  l_combo_profile_status_chooser{FileChooser::get_widget<Gtk::ComboBox>("l_confirm_button", builder)},
  l_confirm_button{FileChooser::get_widget<Gtk::Button>("l_confirm_button", builder)}
  //f_chooser{FileChooser::get_widget<Gtk::FileChooserWidget>("f_chooser", builder)}
{
  
  //l_button->signal_clicked().connect(sigc::mem_fun(*this, &FileChooser::on_button_clicked));
  l_box->set_hexpand();
  l_box->set_vexpand();

  this->add(*l_box);
}

void FileChooser::on_button_clicked(){

  // std::string feedback;
  // std::cout << f_chooser->get_filename() << std::endl;
  // CommandCaller::load_profile(f_chooser->get_filename(), "", feedback);
  // std::cout << feedback << std::endl;
}


