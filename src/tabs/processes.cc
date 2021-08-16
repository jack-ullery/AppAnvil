#include "jsoncpp/json/json.h"
#include "processes.h"
#include "status.h"

#include <iostream>
#include <string>
#include <vector>

// Not finished yet
void Processes::refresh(){
  Json::Value root = Status::get_status_JSON();
  Json::Value processes = root["processes"];

  for(auto proc = processes.begin(); proc != processes.end(); proc++){
    auto row = *(s_model->append());
    row[s_record.s_process] = proc.key().asString();
    // row[s_record.s_status] =  profiles.get(proc.key().asString(), UNKNOWN_STATUS).asString();
  }
}

Processes::Processes()
: s_model{Gtk::ListStore::create(s_record)}
{
  s_view.set_model(s_model);
  
  s_view.append_column("Process", s_record.s_process);
  // s_view.append_column("Status",  s_record.s_status);

  // Make both of those appended columns reorerable.
  for(guint i = 0; i < s_view.get_n_columns(); i++)
  {
    auto *column = s_view.get_column(i);
    column->set_reorderable();
    column->set_sort_column(s_record.s_process);
  }


  refresh();


  this->add(s_view);
  this->show_all();
}

Processes::~Processes() = default;