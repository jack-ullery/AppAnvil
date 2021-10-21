#include "jsoncpp/json/json.h"
#include "profiles.h"

#include <iostream>
#include <string>
#include <vector>

// refresh() is based on assumptions about the output of aa-status.
// If those assumptions are incorrect, or aa-status changes, this could crash.
void Profiles::refresh(){
  Json::Value root = Status::get_status_JSON();
  Json::Value profiles = root["profiles"];

  int num_found = 0;

  col_record->clear();
  for(auto prof = profiles.begin(); prof != profiles.end(); prof++){
    std::string key = prof.key().asString();
    if(filter(key)){
      auto row = col_record->new_row();
      col_record->set_row_data(row, 0, key);
      col_record->set_row_data(row, 1, profiles.get(key, UNKNOWN_STATUS).asString());
      num_found++;
    }
  }

  Status::set_status_label_text(" " + std::to_string(num_found) + " matching profiles");
}

Profiles::Profiles()
: col_record{StatusColumnRecord::create(Status::get_view(), col_names)}
{
  auto func = sigc::mem_fun(*this, &Profiles::on_search_changed);
  Status::set_signal_handler(func);

  this->show_all();
}

void Profiles::on_search_changed(){
  refresh();
}