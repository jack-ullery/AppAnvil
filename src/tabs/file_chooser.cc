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
  l_filechooser_button{FileChooser::get_widget<Gtk::FileChooser>("l_filechooser_button", builder)},
    l_confirm_label{FileChooser::get_widget<Gtk::Label>("l_confirm_label", builder)},
  l_combo_profile_status_chooser{FileChooser::get_widget<Gtk::ComboBoxText>("l_combo_profile_status_chooser", builder)},
  l_confirm_button{FileChooser::get_widget<Gtk::Button>("l_confirm_button", builder)}

{

  l_box->set_hexpand();
  l_box->set_vexpand();
  
  this->add(*l_box);
  auto button_func = sigc::mem_fun(*this, &FileChooser::on_confirm_clicked);
  FileChooser::set_l_button_signal_handler(button_func);
}


void FileChooser::set_l_button_signal_handler(const Glib::SignalProxyProperty::SlotType &func){
        l_confirm_button->signal_clicked().connect(func, true);
}


void FileChooser::on_confirm_clicked(){
  auto choice = l_combo_profile_status_chooser->get_active_text();
  auto file = l_filechooser_button->get_file();
  
    if (file){
  	  auto filename = file->get_path(); 
	  std::string short_filename = CommandCaller::load_profile(filename);
	  
	  std::string choice_lowercase;
	  transform(choice.begin(), choice.end(), choice_lowercase.begin(), ::tolower);
	  if(choice_lowercase != "enforce"){
	    CommandCaller::execute_change(short_filename, "enforce", choice_lowercase);
	  }
	  
	  l_confirm_label->set_text("Done");
	}
}

void FileChooser::clearLabel(){
  l_confirm_label->set_text("");
}
