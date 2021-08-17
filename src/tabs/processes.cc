#include "jsoncpp/json/json.h"
#include "processes.h"
#include "status.h"

#include <iostream>
#include <string>
#include <vector>

template <typename T_Widget>
std::unique_ptr<T_Widget> Processes::get_widget(const Glib::ustring name, const Glib::RefPtr<Gtk::Builder> &builder){
  T_Widget *raw_addr = nullptr;
  builder->get_widget<T_Widget>(name, raw_addr);
  return std::unique_ptr<T_Widget>(raw_addr);
}

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

void Processes::order_columns(){
  for(guint i = 0; i < s_view->get_n_columns(); i++)
  {
    auto *column = s_view->get_column(i);
    column->set_reorderable();
    column->set_sort_column(s_record.s_process);
  }
}

Processes::Processes()
: builder{Gtk::Builder::create_from_resource("/resources/status.glade")},
  s_view{Processes::get_widget<Gtk::TreeView>("s_view", builder)},
  s_win{Processes::get_widget<Gtk::ScrolledWindow>("s_win", builder)},
  s_box{Processes::get_widget<Gtk::Box>("s_box", builder)},
  s_model{Gtk::ListStore::create(s_record)}
{
  s_view->set_model(s_model);
  s_view->append_column("Process", s_record.s_process);

  refresh();
  order_columns();

  s_win->set_shadow_type(Gtk::ShadowType::SHADOW_NONE);
  this->add(*s_box);
  this->show_all();
}
