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
: builder{Gtk::Builder::create_from_file("./resources/file_chooser.glade")},
  f_button{FileChooser::get_widget<Gtk::Button>("f_button", builder)},
  f_box{FileChooser::get_widget<Gtk::Box>("f_box", builder)},
  f_chooser{FileChooser::get_widget<Gtk::FileChooserWidget>("f_chooser", builder)}
{
  
  f_button->signal_clicked().connect(sigc::mem_fun(*this, &FileChooser::on_button_clicked));
  f_box->set_hexpand();
  f_box->set_vexpand();

  this->add(*f_box);
}

void FileChooser::on_button_clicked(){

  std::string feedback;
  std::cout << f_chooser->get_filename() << std::endl;
  CommandCaller::load_profile(f_chooser->get_filename(), "", feedback);
  std::cout << feedback << std::endl;
}


