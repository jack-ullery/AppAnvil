#include "jsoncpp/json/json.h"
#include "profiles.h"
#include "status.h"

#include <iostream>
#include <string>
#include <vector>

// refresh() is based on assumptions about the output of aa-status.
// If those assumptions are incorrect, or aa-status changes, this could crash.
void Profiles::refresh(){
  Json::Value root = Status::get_status_JSON();
  Json::Value profiles = root["profiles"];

  for(auto prof = profiles.begin(); prof != profiles.end(); prof++){
    auto row = *(s_model->append());
    row[s_record.s_profile] = prof.key().asString();
    row[s_record.s_status] =  profiles.get(prof.key().asString(), UNKNOWN_STATUS).asString();
  }
}

Profiles::Profiles()
: s_model{Gtk::ListStore::create(s_record)}
{
  s_view.set_model(s_model);
  
  s_view.append_column("Profile", s_record.s_profile);
  s_view.append_column("Status",  s_record.s_status);

  // Make both of those appended columns reorerable.
  for(guint i = 0; i < s_view.get_n_columns(); i++)
  {
    auto *column = s_view.get_column(i);
    column->set_reorderable();
    // column->set_sort_column(s_record.s_profile);
    column->set_sort_column(s_record.s_status);
  }


  refresh();


  this->add(s_view);
  this->show_all();
}

Profiles::~Profiles() = default;