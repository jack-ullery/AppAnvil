#include "jsoncpp/json/json.h"
#include "processes.h"
#include "status.h"

#include <iostream>
#include <regex>
#include <string>
#include <vector>

template <typename T_Widget>
std::unique_ptr<T_Widget> Processes::get_widget(const Glib::ustring name, const Glib::RefPtr<Gtk::Builder> &builder){
  T_Widget *raw_addr = nullptr;
  builder->get_widget<T_Widget>(name, raw_addr);
  return std::unique_ptr<T_Widget>(raw_addr);
}

bool Processes::filter(const std::string& str, const std::string& rule){

  if(str.find(rule) != std::string::npos){
    return true;
  }

  try{
    std::regex filter_regex(rule);
    if(std::regex_match(str, filter_regex)){
      return true;
    }
  } catch(const std::regex_error& ex){  }

  return false;
}

// Not finished yet. Need to add information for PID somewhere.
void Processes::refresh(const std::string& rule){
  Json::Value root = Status::get_status_JSON();
  Json::Value processes = root["processes"];

  list_store->clear();
  for(auto proc = processes.begin(); proc != processes.end(); proc++){
    std::string key = proc.key().asString();
    if(filter(key, rule)){
      auto row = *(list_store->append());
      row[s_record.s_process] = proc.key().asString();
      // row[s_record.s_status] =  profiles.get(proc.key().asString(), UNKNOWN_STATUS).asString();
    }
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
  s_search{Processes::get_widget<Gtk::SearchEntry>("s_search", builder)},
  list_store{Gtk::ListStore::create(s_record)}
{
  s_view->set_model(list_store);
  s_view->append_column("Process", s_record.s_process);

  refresh(".*");
  order_columns();

  s_win->set_shadow_type(Gtk::ShadowType::SHADOW_NONE);
  s_search->signal_search_changed().connect(
    sigc::mem_fun(*this, &Processes::on_search_changed), true);
  this->add(*s_box);
  this->show_all();
}

void Processes::on_search_changed(){
  refresh(s_search->get_text());
}