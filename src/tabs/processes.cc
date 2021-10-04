#include "jsoncpp/json/json.h"
#include "processes.h"

#include <gtkmm/enums.h>
#include <iostream>
#include <regex>
#include <string>

const std::regex filter_unconfined_proc("\\b([1234567890]+) ([^ ]+) (not confined)");

// Need to improve!
void Processes::refresh(){
  Json::Value root = Status::get_status_JSON();
  Json::Value processes = root["processes"];

  int num_found = 0;
  col_record->clear();
  for(auto proc = processes.begin(); proc != processes.end(); proc++){
    const std::string& key = proc.key().asString();
    if(filter(key)){
      auto row = col_record->new_column();
      row[col_record->column[0]] = key;

      Json::Value val = (*proc)[0];
      for(auto inst = proc->begin(); inst != proc->end(); inst++){
        auto child = col_record->new_child_column(row);
        child[col_record->column[0]] = "pid: " + inst->get("pid", "Unknown").asString() + "\t status: " + inst->get("status", "Unknown").asString();
      }
      num_found++;
    }
  }

  std::stringstream unconfined_results;
  std::string line;
  unconfined_results << Status::get_unconfined();
  while(std::getline(unconfined_results, line)){
    std::smatch m;
    bool re = regex_search(line, m, filter_unconfined_proc);
    if(re){
      std::string prof_name = m[2].str();
      std::string pid = m[1].str();

      if(filter(prof_name)){
        auto row = col_record->new_column();
        row[col_record->column[0]] = m[2].str();

        auto child = col_record->new_child_column(row);
        child[col_record->column[0]] = "pid: " + m[1].str() + "\t status: " + "unconfined";
        num_found++;
      }
    }
  }

  s_found_label->set_text(" " + std::to_string(num_found) + " matching processes");
}

Processes::Processes()
: col_record{StatusColumnRecord::create(s_view, col_names)}
{
  refresh();

  auto sig_handler = sigc::mem_fun(*this, &Processes::on_search_changed);
  s_search->signal_search_changed().connect(sig_handler, true);
  s_use_regex->signal_clicked().connect(sig_handler, true);
  s_match_case->signal_clicked().connect(sig_handler, true);
  s_whole_word->signal_clicked().connect(sig_handler, true);

  this->show_all();
}

void Processes::on_search_changed(){
  refresh();
}