#include "jsoncpp/json/json.h"
#include "processes.h"

#include <gtkmm/enums.h>
#include <iostream>
#include <regex>
#include <string>

void Processes::refresh(){
  Json::Value root = Status::get_status_JSON();
  Json::Value processes = root["processes"];

  int num_found = 0;
  // std::cout << "Root: \n\t" << root << std::endl;
  tree_store->clear();
  for(auto proc = processes.begin(); proc != processes.end(); proc++){
    const std::string& key = proc.key().asString();
    if(filter(key)){
      auto row = *(tree_store->append());
      row[s_record.s_process] = key;

      Json::Value val = (*proc)[0];
      for(auto inst = proc->begin(); inst != proc->end(); inst++){
        auto child = *(tree_store->append(row.children()));
        child[s_record.s_process] = "pid: " + inst->get("pid", "Unknown").asString() + "\t profile: "+inst->get("profile", "Unknown").asString()+"\t status: " + inst->get("status", "Unknown").asString();
      }
      num_found++;
    }
    s_found_label->set_text(" " + std::to_string(num_found) + " matching processes");
  }
}

void Processes::order_columns(){
  auto *column = s_view->get_column(0);
  column->set_reorderable();
  column->set_resizable();
  column->set_min_width(MIN_COL_WIDTH);
  column->set_sort_column(s_record.s_process);
}

Processes::Processes()
: tree_store{Gtk::TreeStore::create(s_record)}
{
  s_view->set_model(tree_store);
  s_view->append_column("Process", s_record.s_process);

  refresh();
  order_columns();

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