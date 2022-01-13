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
  l_upper_text{FileChooser::get_widget<Gtk::Label>("l_upper_text", builder)},
  l_mid_grid{FileChooser::get_widget<Gtk::Grid>("l_mid_grid", builder)},
  l_filechooser_button{FileChooser::get_widget<Gtk::Button>("l_filechooser_button", builder)},
  l_filechooser_label{FileChooser::get_widget<Gtk::Label>("l_filechooser_label", builder)},
  l_lower_text{FileChooser::get_widget<Gtk::Label>("l_lower_text", builder)},
  l_lower_grid{FileChooser::get_widget<Gtk::Grid>("l_lower_grid", builder)},
  l_combo_profile_status_chooser{FileChooser::get_widget<Gtk::ComboBoxText>("l_combo_profile_status_chooser", builder)},
  l_confirm_button{FileChooser::get_widget<Gtk::Button>("l_confirm_button", builder)}
  //f_chooser{FileChooser::get_widget<Gtk::FileChooserWidget>("f_chooser", builder)}
{
  
  
  l_filechooser_button->signal_clicked().connect(sigc::mem_fun(*this, &FileChooser::on_fc_button_clicked));
  l_confirm_button->signal_clicked().connect(sigc::mem_fun(*this, &FileChooser::on_confirm_clicked));
  l_box->set_hexpand();
  l_box->set_vexpand();
  this->add(*l_box);
}



void FileChooser::on_fc_button_clicked(){


  // Associate the f_diag pointer with a new Dialog
  f_diag.reset(new Gtk::FileChooserDialog("Choose a profile", Gtk::FILE_CHOOSER_ACTION_OPEN));
  // Close dialog when the OK button is pressed.

  f_diag->add_button("Confirm", Gtk::ResponseType::RESPONSE_OK);
  f_diag->signal_response().connect(sigc::mem_fun(*this, &FileChooser::handle_f_diag));
  f_diag->show();
  
}

void FileChooser::handle_f_diag(int response_id){
  auto filename = f_diag->get_file()->get_path();
  l_filechooser_label->set_text(filename);
  f_diag->hide();
}

void FileChooser::on_confirm_clicked(){
  auto choice = l_combo_profile_status_chooser->get_active_text();
  auto filename = l_filechooser_label->get_text();

  std::string short_filename = CommandCaller::load_profile(filename);
  
  std::string choice_lowercase;
  transform(choice.begin(), choice.end(), choice_lowercase.begin(), ::tolower);
  if(choice_lowercase != "enforce"){
    CommandCaller::execute_change(short_filename, "enforce", choice_lowercase);
  }
}


