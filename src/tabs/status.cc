#include "jsoncpp/json/json.h"
#include "status.h"

#include <iostream>
#include <regex>
#include <sstream>
#include <string>


template <typename T_Widget>
std::unique_ptr<T_Widget> Status::get_widget(Glib::ustring name, const Glib::RefPtr<Gtk::Builder> &builder){
  T_Widget *raw_addr = nullptr;
  builder->get_widget<T_Widget>(name, raw_addr);
  return std::unique_ptr<T_Widget>(raw_addr);
}

bool Status::filter(const std::string& str, const std::string& rule, const bool& use_regex, const bool& match_case, const bool& whole_word){
  std::string new_str = str;
  std::string new_rule = rule;

  if(!match_case){
    // If we don't care about case, convert the filtered string and rule to lower case
    transform(str.begin(), str.end(), new_str.begin(), ::tolower);
    transform(rule.begin(), rule.end(), new_rule.begin(), ::tolower);
  }

  if(use_regex){
    try{
      // Try to parse the regular expression rule.
      std::regex filter_regex(new_rule);
      if(whole_word){
        // Match strings that entirely match the rule
        return std::regex_match(new_str, filter_regex);
      }
      // Otherwise, match strings that contain a substring that matches the rule
      return std::regex_search(new_str, filter_regex);

    } catch(const std::regex_error& ex){
      // If the regular expression was not valid, return false
      return false;
    }
  }

  if(whole_word){
    // Only match strings that are exactly equal to the rule
    return new_str == new_rule;
  }

  // Otherwise, match any string that contains the rule as a substring
  return new_str.find(new_rule) != std::string::npos;    
}

bool Status::filter(const Gtk::TreeModel::iterator& node){
  std::string data;
  node->get_value(0, data);
  return Status::filter(data, s_search->get_text(), s_use_regex->get_active(), s_match_case->get_active(), s_whole_word->get_active());
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

void Status::set_status_label_text(const std::string& str){
  s_found_label->set_text(str);
}

void Status::set_refresh_signal_handler(const Glib::SignalProxyProperty::SlotType& func){
  s_search->signal_search_changed().connect(func, true);
  s_use_regex->signal_clicked().connect(func, true);
  s_match_case->signal_clicked().connect(func, true);
  s_whole_word->signal_clicked().connect(func, true);
}

// for handling the apply button being clicked when an appopriate choice has been made in the drop-down
void Status::set_apply_signal_handler(const Glib::SignalProxyProperty::SlotType& func){
  s_apply_button->signal_clicked().connect(func, true);
}

std::shared_ptr<Gtk::TreeView> Status::get_view(){
  return s_view;
}

void Status::remove_status_selection(){
  s_box->remove(*s_selection_box);
  s_selection_box->hide();
}

Status::Status()
: builder{Gtk::Builder::create_from_resource("/resources/status.glade")},
  s_view{Status::get_widget<Gtk::TreeView>("s_view", builder)},
  s_win{Status::get_widget<Gtk::ScrolledWindow>("s_win", builder)},
  s_box{Status::get_widget<Gtk::Box>("s_box", builder)},
  s_search{Status::get_widget<Gtk::SearchEntry>("s_search", builder)},
  s_use_regex{Status::get_widget<Gtk::CheckButton>("s_use_regex", builder)},
  s_match_case{Status::get_widget<Gtk::CheckButton>("s_match_case", builder)},
  s_whole_word{Status::get_widget<Gtk::CheckButton>("s_whole_word", builder)},
  s_found_label{Status::get_widget<Gtk::Label>("s_found_label", builder)},
  s_selection_box{Status::get_widget<Gtk::Box>("s_selection_box", builder)},
  s_apply_button{Status::get_widget<Gtk::Button>("s_apply_button", builder)},
  s_spinner{Status::get_widget<Gtk::Spinner>("s_spinner", builder)},
  s_apply_info_text{Status::get_widget<Gtk::Label>("s_apply_info_text", builder)},
  s_status_selection{Status::get_widget<Gtk::ComboBox>("s_status_selection", builder)}
{
  s_win->set_shadow_type(Gtk::ShadowType::SHADOW_NONE);
  s_win->set_policy(Gtk::PolicyType::POLICY_AUTOMATIC, Gtk::PolicyType::POLICY_AUTOMATIC);
  s_win->set_hexpand();
  s_win->set_vexpand();

  this->add(*s_box);
}