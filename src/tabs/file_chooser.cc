#include "jsoncpp/json/json.h"
#include "file_chooser.h"

#include <iostream>
#include <string>
#include <vector>

// add_data_to_record() is based on assumptions about the output of aa-status.
// If those assumptions are incorrect, or aa-status changes, this could crash.
void FileChooser::add_data_to_record(std::string data){
  Json::Value root = Status::parse_JSON(data);
  Json::Value filechooser = root["filechooser"];

  int num_found = 0;

  col_record->clear();
  for(auto prof = filechooser.begin(); prof != filechooser.end(); prof++){
    std::string key = prof.key().asString();
    auto row = col_record->new_row();
    col_record->set_row_data(row, 0, key);
    col_record->set_row_data(row, 1, filechooser.get(key, UNKNOWN_STATUS).asString());
    num_found++;
  }

  Status::set_status_label_text(" " + std::to_string(num_found) + " matching filechooser");
}

void FileChooser::change_status(){
  //s_row = s_view.get_selection();
  std::cout << "button works\n" << std::endl;
}

void FileChooser::refresh(){
  std::cout << "Refresh called!" << std::endl;
  auto filter_fun = sigc::mem_fun(*this, &FileChooser::filter);
  col_record->filter_rows(filter_fun);
}

FileChooser::FileChooser()
: builder{Gtk::Builder::create_from_file("./resources.file_chooser.glade")}
{
  //get main widgets
  builder->get_widget<Gtk::Button>("f_button", f_button);
  builder->get_widget<Gtk::Box>("f_box", f_box);
  
  auto refresh_func = sigc::mem_fun(*this, &FileChooser::refresh);
  auto apply_func = sigc::mem_fun(*this, &FileChooser::on_apply_button_pressed);
  Status::set_refresh_signal_handler(refresh_func);
  Status::set_apply_signal_handler(apply_func);

  this->show_all();
}

void FileChooser::on_apply_button_pressed(){
  change_status();
}