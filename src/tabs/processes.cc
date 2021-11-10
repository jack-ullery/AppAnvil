#include "jsoncpp/json/json.h"
#include "processes.h"

#include <gtkmm/enums.h>
#include <iostream>
#include <regex>
#include <string>

// clang-tidy throws [cert-err58-cpp], but it's not a problem in this case, so lets ignore it.
const std::regex filter_unconfined_proc("\\b([1234567890]+) ([^ ]+) (not confined)"); // NOLINT(cert-err58-cpp)

// Need to improve!
void Processes::add_data_to_record(std::string confined, std::string unconfined){
  Json::Value root = Status::parse_JSON(confined);
  Json::Value processes = root["processes"];

  int num_found = 0;
  col_record->clear();
  for(auto proc = processes.begin(); proc != processes.end(); proc++){
    const std::string& key = proc.key().asString();
    auto row = col_record->new_row();
    col_record->set_row_data(row, 0, key);

    Json::Value val = (*proc)[0];
    for(auto inst = proc->begin(); inst != proc->end(); inst++){
      auto child = col_record->new_child_row(row);
      col_record->set_row_data(child, 0, "pid: " + inst->get("pid", "Unknown").asString() + "\t status: " + inst->get("status", "Unknown").asString());
    }
  
    num_found++;
  }

  std::stringstream unconfined_results;
  std::string line;
  unconfined_results << unconfined;
  while(std::getline(unconfined_results, line)){
    std::smatch m;
    bool re = regex_search(line, m, filter_unconfined_proc);
    if(re){
      std::string prof_name = m[2].str();
      std::string pid = m[1].str();

      auto row = col_record->new_row();
      col_record->set_row_data(row, 0,  m[2].str());
      
      auto child = col_record->new_child_row(row);
      col_record->set_row_data(child, 0, "pid: " + m[1].str() + "\t status: " + "unconfined");

      num_found++;
    }
  }

  Status::set_status_label_text(" " + std::to_string(num_found) + " matching processes");
}

void Processes::refresh(){
  col_record->filter_rows();
}

Processes::Processes()
: col_record{StatusColumnRecord::create(Status::get_view(), col_names)}
{
  auto func = sigc::mem_fun(*this, &Processes::refresh);
  Status::set_refresh_signal_handler(func);

  auto filter_fun = sigc::mem_fun(*this, &Processes::filter);
  col_record->set_visible_func(filter_fun);

  this->show_all();
}
