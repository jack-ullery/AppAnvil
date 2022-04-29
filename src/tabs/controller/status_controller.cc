#include "status_controller.h"
#include "../model/status_column_record.h"
#include "../search_info.h"
#include "../view/logs.h"
#include "../view/processes.h"
#include "../view/profiles.h"

#include "jsoncpp/json/json.h"

#include <iostream>
#include <regex>
#include <sstream>
#include <string>

template<class Tab>
bool StatusController<Tab>::filter(const std::string &str, const std::string &rule, const bool &use_regex, const bool &match_case, const bool &whole_word)
{
  std::string new_str  = str;
  std::string new_rule = rule;

  if(use_regex) {
    try {
      // Set a flag if we want to ignore case
      auto flag = (match_case) ? (std::regex_constants::collate) : (std::regex_constants::collate | std::regex_constants::icase);
      // Try to parse the regular expression rule.
      std::regex filter_regex(new_rule, flag);

      if(whole_word) {
        // Match strings that entirely match the rule
        return std::regex_match(new_str, filter_regex);
      }

      // Otherwise, match strings that contain a substring that matches the rule
      return std::regex_search(new_str, filter_regex);

    } catch(const std::regex_error &ex) {
      // If the regular expression was not valid, return false
      return false;
    }
  } else if(!match_case) {
    // If we don't care about case and we are not using regex, convert the filtered string and rule to lower case
    transform(str.begin(), str.end(), new_str.begin(), ::tolower);
    transform(rule.begin(), rule.end(), new_rule.begin(), ::tolower);
  }

  if(whole_word) {
    // Only match strings that are exactly equal to the rule
    return new_str == new_rule;
  }

  // Otherwise, match any string that contains the rule as a substring
  return new_str.find(new_rule) != std::string::npos;
}

template<class Tab>
bool StatusController<Tab>::filter(const Gtk::TreeModel::iterator &node)
{
  std::string data;
  const uint num_columns = tab->get_view()->get_n_columns();
  auto treeModel         = tab->get_view()->get_model();

  for(uint i = 0; i < num_columns; i++) {
      // SearchInfo info = tab->get_search_info();
      SearchInfo info("", false, false, false);
      bool re               = false;
      unsigned int uintData = 0;

    if(treeModel->get_column_type(i) == COLUMN_TYPE_STRING) {
      node->get_value(i, data);
      re = StatusController<Tab>::filter(data, info.filter_rule, info.use_regex, info.match_case, info.whole_word);
    } else {
      node->get_value(i, uintData);
      re = StatusController<Tab>::filter(std::to_string(uintData), info.filter_rule, info.use_regex, info.match_case, info.whole_word);
    }

    if(re) {
      return true;
    }
  }

  // auto children = node->children();
  // if(!children.empty()) {
  //   return filter_children(node);
  // }

  return false;
}

template<class Tab>
bool StatusController<Tab>::filter_children(const Gtk::TreeModel::iterator &node)
{
  // std::string data;
  // const uint num_columns = tab->get_view()->get_n_columns();
  // auto treeModel         = tab->get_view()->get_model();
  // auto children          = node->children();

  // for(auto iter = children.begin(); iter != children.end(); iter++) {
  //   auto row = *iter;
  //   for(uint i = 0; i < num_columns; i++) {
  //     SearchInfo info = tab->get_search_info();
  //     bool re               = false;
  //     unsigned int uintData = 0;

  //     if(treeModel->get_column_type(i) == COLUMN_TYPE_STRING) {
  //       row.get_value(i, data);
  //       re = StatusController<Tab>::filter(data, info.filter_rule, info.use_regex, info.match_case, info.whole_word);
  //     } else {
  //       row.get_value(i, uintData);
  //       re = StatusController<Tab>::filter(std::to_string(uintData), info.filter_rule, info.use_regex, info.match_case, info.whole_word);
  //     }

  //     if(re) {
  //       return true;
  //     }

  //     if(!row.children().empty() && filter_children(row)) {
  //       return true;
  //     }
  //   }
  // }

  return false;
}

template<class Tab>
Json::Value StatusController<Tab>::parse_JSON(const std::string &raw_json)
{
  std::stringstream stream;
  stream << raw_json;

  Json::Value root;
  Json::CharReaderBuilder builder;
  JSONCPP_STRING errs;

  if(!parseFromStream(builder, stream, &root, &errs)) {
    throw std::invalid_argument(errs + "\nArgument of parse_JSON is not in valid JSON format.");
  }

  return root;
}

template<class Tab>
StatusController<Tab>::StatusController()
: tab{new Tab()}
{ }

template<class Tab>
std::shared_ptr<Tab> StatusController<Tab>::get_tab(){
  return tab;
}

template class StatusController<Profiles<StatusColumnRecord>>;
template class StatusController<Processes<StatusColumnRecord>>;
template class StatusController<Logs>;
