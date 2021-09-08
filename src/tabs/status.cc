#include "jsoncpp/json/json.h"
#include "status.h"

#include <giomm.h>
#include <glibmm.h>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <string>
#include <vector>


template <typename T_Widget>
std::unique_ptr<T_Widget> Status::get_widget(const Glib::ustring name, const Glib::RefPtr<Gtk::Builder> &builder){
  T_Widget *raw_addr = nullptr;
  builder->get_widget<T_Widget>(name, raw_addr);
  return std::unique_ptr<T_Widget>(raw_addr);
}

bool Status::filter(const std::string& str, const std::string& rule){

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

Json::Value Status::parse_JSON(const std::string& raw_json){
  std::stringstream stream;
  stream << raw_json;

  Json::Value root;
  Json::CharReaderBuilder builder;
  JSONCPP_STRING errs;
  
  if (!parseFromStream(builder, stream, &root, &errs)) {
    std::cout << errs << std::endl;
  }

  return root;
}

std::string Status::get_status_str(){
  std::vector<std::string> args = {"sudo", "aa-status","--json"};

  std::string child_output;
  std::string child_error;
  int exit_status = 0;

  Glib::spawn_sync("/usr/sbin/", args, Glib::SpawnFlags::SPAWN_DEFAULT, {}, &child_output, &child_error, &exit_status);

  if(exit_status != 0){
    std::cout << "Error calling '"<< args[0] <<"'. " << child_error << std::endl;
    child_output = "{\"processes\": {}, \"profiles\": {}";
  }

  return child_output;
}

Json::Value Status::get_status_JSON(){
  return parse_JSON(get_status_str());
}

Status::Status()
: builder{Gtk::Builder::create_from_resource("/resources/status.glade")},
  s_view{Status::get_widget<Gtk::TreeView>("s_view", builder)},
  s_win{Status::get_widget<Gtk::ScrolledWindow>("s_win", builder)},
  s_box{Status::get_widget<Gtk::Box>("s_box", builder)},
  s_search{Status::get_widget<Gtk::SearchEntry>("s_search", builder)}
{
  s_win->set_shadow_type(Gtk::ShadowType::SHADOW_NONE);
  s_win->set_policy(Gtk::PolicyType::POLICY_AUTOMATIC, Gtk::PolicyType::POLICY_AUTOMATIC);
  s_win->set_hexpand();
  s_win->set_vexpand();

  this->add(*s_box);
}