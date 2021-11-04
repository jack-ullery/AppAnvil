#include "jsoncpp/json/json.h"
#include "profiles.h"

#include <iostream>
#include <string>
#include <vector>

// add_data_to_record() is based on assumptions about the output of aa-status.
// If those assumptions are incorrect, or aa-status changes, this could crash.
void Profiles::add_data_to_record(std::string data){
  Json::Value root = Status::parse_JSON(data);
  Json::Value profiles = root["profiles"];

  int num_found = 0;

  col_record->clear();
  for(auto prof = profiles.begin(); prof != profiles.end(); prof++){
    std::string key = prof.key().asString();
    auto row = col_record->new_row();
    col_record->set_row_data(row, 0, key);
    col_record->set_row_data(row, 1, profiles.get(key, UNKNOWN_STATUS).asString());
    num_found++;
  }

  Status::set_status_label_text(" " + std::to_string(num_found) + " matching profiles");
}

void Profiles::change_status(){
  //s_row = s_view.get_selection();
  std::cout << "button works\n" << std::endl;
}

void Profiles::refresh(){
  std::cout << "Refresh called!" << std::endl;
  auto filter_fun = sigc::mem_fun(*this, &Profiles::filter);
  col_record->filter_rows(filter_fun);
}

Profiles::Profiles()
: col_record{StatusColumnRecord::create(Status::get_view(), col_names)}
{
  auto refresh_func = sigc::mem_fun(*this, &Profiles::refresh);
  auto apply_func = sigc::mem_fun(*this, &Profiles::on_apply_button_pressed);
  Status::set_refresh_signal_handler(refresh_func);
  Status::set_apply_signal_handler(apply_func);

  this->show_all();
}

void Profiles::on_apply_button_pressed(){
  change_status();
}