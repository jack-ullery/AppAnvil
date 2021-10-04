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
      auto row = col_record->new_column();
      row[col_record->column[0]] = key;
      row[col_record->column[1]] =  profiles.get(key, UNKNOWN_STATUS).asString();
      num_found++;
    }
  }

  s_found_label->set_text(" " + std::to_string(num_found) + " matching profiles");
}

Profiles::Profiles()
: col_record{StatusColumnRecord::create(s_view, col_names)}
{
  refresh();

  auto sig_handler = sigc::mem_fun(*this, &Profiles::on_search_changed);
  s_search->signal_search_changed().connect(sig_handler, true);
  s_use_regex->signal_clicked().connect(sig_handler, true);
  s_match_case->signal_clicked().connect(sig_handler, true);
  s_whole_word->signal_clicked().connect(sig_handler, true);

  this->show_all();
}

void Profiles::on_search_changed(){
  refresh();
}