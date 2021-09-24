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

  list_store->clear();
  for(auto prof = profiles.begin(); prof != profiles.end(); prof++){
    std::string key = prof.key().asString();
    if(filter(key)){
      auto row = *(list_store->append());
      row[col_record.profile_col] = key;
      row[col_record.status_col] =  profiles.get(key, UNKNOWN_STATUS).asString();
      num_found++;
    }
  }

  s_found_label->set_text(" " + std::to_string(num_found) + " matching profiles");
}

void Profiles::order_columns(){
  // Notice the column retrieved is a TreeViewColumn, not a TreeModelColumn like was used with s_record
  // The column numbers depend on the order the are added to s_view
  auto *profile_view_col = s_view->get_column(0);
  profile_view_col->set_reorderable();
  profile_view_col->set_resizable();
  profile_view_col->set_min_width(MIN_COL_WIDTH);
  profile_view_col->set_sort_column(col_record.profile_col);

  auto *status_view_col = s_view->get_column(1);
  status_view_col->set_reorderable();
  status_view_col->set_resizable();
  status_view_col->set_min_width(MIN_COL_WIDTH);
  status_view_col->set_sort_column(col_record.status_col);
}

Profiles::Profiles()
:  list_store{Gtk::ListStore::create(col_record)}
{
  s_view->set_model(list_store);
  s_view->append_column("Profile", col_record.profile_col);
  s_view->append_column("Status", col_record.status_col);

  refresh();
  order_columns();

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