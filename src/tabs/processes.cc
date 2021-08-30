#include "jsoncpp/json/json.h"
#include "processes.h"
#include "status.h"

#include <iostream>
#include <regex>
#include <gtkmm/enums.h>
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

// Not finished yet. Want to improve how the columns with the children look.
void Processes::refresh(const std::string& rule){
  Json::Value root = Status::get_status_JSON();
  Json::Value processes = root["processes"];

  // std::cout << "Root: \n\t" << root << std::endl;
  tree_store->clear();
  for(auto proc = processes.begin(); proc != processes.end(); proc++){
    const std::string& key = proc.key().asString();
    if(filter(key, rule)){
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
: builder{Gtk::Builder::create_from_resource("/resources/status.glade")},
  s_view{Processes::get_widget<Gtk::TreeView>("s_view", builder)},
  s_win{Processes::get_widget<Gtk::ScrolledWindow>("s_win", builder)},
  s_box{Processes::get_widget<Gtk::Box>("s_box", builder)},
  s_search{Processes::get_widget<Gtk::SearchEntry>("s_search", builder)},
  tree_store{Gtk::TreeStore::create(s_record)}
{
  s_view->set_model(tree_store);
  s_view->append_column("Process", s_record.s_process);
  s_view->append_column("Profile", s_record.s_profile);
  s_view->append_column("Status", s_record.s_status);

  s_win->set_policy(Gtk::PolicyType::POLICY_AUTOMATIC, Gtk::PolicyType::POLICY_AUTOMATIC);
  s_win->set_hexpand();
  s_win->set_vexpand();

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