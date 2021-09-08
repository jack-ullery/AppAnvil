#include "jsoncpp/json/json.h"
#include "profiles.h"

#include <iostream>
#include <string>
#include <vector>

// refresh() is based on assumptions about the output of aa-status.
// If those assumptions are incorrect, or aa-status changes, this could crash.
void Profiles::refresh(const std::string& rule){
  Json::Value root = Status::get_status_JSON();
  Json::Value profiles = root["profiles"];

  list_store->clear();
  for(auto prof = profiles.begin(); prof != profiles.end(); prof++){
    std::string key = prof.key().asString();
    if(Status::filter(key, rule)){
      auto row = *(list_store->append());
      row[col_record.profile_col] = key;
      row[col_record.status_col] =  profiles.get(key, UNKNOWN_STATUS).asString();
    }
  }
}

void Profiles::order_columns(){
  auto *profile_view_col = s_view->get_column(0);
  profile_view_col->set_reorderable();
  profile_view_col->set_sort_column(col_record.profile_col);

  auto *status_view_col = s_view->get_column(1);
  status_view_col->set_reorderable();
  status_view_col->set_sort_column(col_record.status_col);
}

Profiles::Profiles()
:  list_store{Gtk::ListStore::create(col_record)}
{
  s_view->set_model(list_store);
  s_view->append_column("Profile", col_record.profile_col);
  s_view->append_column("Status", col_record.status_col);

  refresh(".*");
  order_columns();

  s_search->signal_search_changed().connect(
    sigc::mem_fun(*this, &Profiles::on_search_changed), true);
  this->show_all();
}

void Profiles::on_search_changed(){
  refresh(s_search->get_text());
}