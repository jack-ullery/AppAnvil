#include "jsoncpp/json/json.h"
#include "processes.h"

#include <gtkmm/enums.h>
#include <iostream>
#include <regex>
#include <string>

// Not finished yet. Want to improve how the columns with the children look.
void Processes::refresh(const std::string& rule){
  Json::Value root = Status::get_status_JSON();
  Json::Value processes = root["processes"];

  // std::cout << "Root: \n\t" << root << std::endl;
  tree_store->clear();
  for(auto proc = processes.begin(); proc != processes.end(); proc++){
    const std::string& key = proc.key().asString();
    if(Status::filter(key, rule)){
      auto row = *(tree_store->append());
      row[s_record.s_process] = key;

      Json::Value val = (*proc)[0];
      for(auto inst = proc->begin(); inst != proc->end(); inst++){
        auto child = *(tree_store->append(row.children()));
        child[s_record.s_process] = "pid: " + inst->get("pid", "Unknown").asString();
        child[s_record.s_profile] = inst->get("profile", "Unknown").asString();
        child[s_record.s_status]  = inst->get("status", "Unknown").asString();
      }
    }
  }
}

void Processes::order_columns(){
  // Notice the column retrieved is a TreeViewColumn, not a TreeModelColumn like was used with s_record
  // The column numbers depend on the order the are added to s_view
  auto *column = s_view->get_column(0);
  column->set_reorderable();
  column->set_sort_column(s_record.s_process);

  column = s_view->get_column(1);
  column->set_reorderable();
  column->set_sort_column(s_record.s_profile);

  column = s_view->get_column(2);
  column->set_reorderable();
  column->set_sort_column(s_record.s_status);
}

Processes::Processes()
: tree_store{Gtk::TreeStore::create(s_record)}
{
  s_view->set_model(tree_store);
  s_view->append_column("Process", s_record.s_process);
  s_view->append_column("Profile", s_record.s_profile);
  s_view->append_column("Status", s_record.s_status);

  refresh(".*");
  order_columns();

  s_search->signal_search_changed().connect(
    sigc::mem_fun(*this, &Processes::on_search_changed), true);
  this->show_all();
}

void Processes::on_search_changed(){
  refresh(s_search->get_text());
}