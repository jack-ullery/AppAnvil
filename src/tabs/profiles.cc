#include "jsoncpp/json/json.h"
#include "profiles.h"
#include "status.h"

#include <iostream>
#include <string>
#include <vector>

template <typename T_Widget>
std::unique_ptr<T_Widget> Profiles::get_widget(const Glib::ustring name, const Glib::RefPtr<Gtk::Builder> &builder){
  T_Widget *raw_addr = nullptr;
  builder->get_widget<T_Widget>(name, raw_addr);
  return std::unique_ptr<T_Widget>(raw_addr);
}

// refresh() is based on assumptions about the output of aa-status.
// If those assumptions are incorrect, or aa-status changes, this could crash.
void Profiles::refresh(){
  Json::Value root = Status::get_status_JSON();
  Json::Value profiles = root["profiles"];

  for(auto prof = profiles.begin(); prof != profiles.end(); prof++){
    auto row = *(list_store->append());
    row[col_record.profile_col] = prof.key().asString();
    row[col_record.status_col] =  profiles.get(prof.key().asString(), UNKNOWN_STATUS).asString();
  }
}

void Profiles::order_columns(){
  auto profile_view_col = s_view->get_column(0);
  profile_view_col->set_reorderable();
  profile_view_col->set_sort_column(col_record.profile_col);

  auto status_view_col = s_view->get_column(1);
  status_view_col->set_reorderable();
  status_view_col->set_sort_column(col_record.status_col);
}

Profiles::Profiles()
: builder{Gtk::Builder::create_from_resource("/resources/status.glade")},
  s_view{Profiles::get_widget<Gtk::TreeView>("s_view", builder)},
  s_win{Profiles::get_widget<Gtk::ScrolledWindow>("s_win", builder)},
  s_box{Profiles::get_widget<Gtk::Box>("s_box", builder)},
  list_store{Gtk::ListStore::create(col_record)}
{
  s_view->set_model(list_store);

  s_view->append_column("Profile", col_record.profile_col);
  s_view->append_column("Status", col_record.status_col);

  refresh();
  order_columns();

  s_win->set_shadow_type(Gtk::ShadowType::SHADOW_NONE);
  this->add(*s_box);
  this->show_all();
}
