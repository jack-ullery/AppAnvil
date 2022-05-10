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
bool StatusController<Tab>::should_filter(const std::string &str, const std::string &rule, const bool &use_regex, const bool &match_case, const bool &whole_word)
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
  const uint num_columns = tab->get_view()->get_n_columns();
  auto treeModel         = tab->get_view()->get_model();
  SearchInfo info        = tab->get_search_info();

  // If one column of the tree row has a string that matches the pattern, then make this node visible
  for(uint i = 0; i < num_columns; i++) {
    bool re  = false;

    if(treeModel->get_column_type(i) == COLUMN_TYPE_STRING) {
      // If the column is a string, see whether it matches the pattern
      std::string data;
      node->get_value(i, data);
      re = StatusController<Tab>::should_filter(data, info.filter_rule, info.use_regex, info.match_case, info.whole_word);
    } else {
      // If the column is an int, convert it to a string and see whether it matches the pattern
      // We initialize this uint to avoid a warning, this value will be overwritten by the following line
      unsigned int data = 0;
      node->get_value(i, data);
      re = StatusController<Tab>::should_filter(std::to_string(data), info.filter_rule, info.use_regex, info.match_case, info.whole_word);
    }

    // If the data in this column should be visible, show the entire row
    if(re) {
      return true;
    }
  }

  // Search the children and determine whether or not they should be visible
  // For efficiency: we should use a more complex data structure, or do memoization
  auto children = node->children();
  for(auto child = children.begin(); child != children.end(); child++) {
    bool re = filter(child);
    // If a child of this row is visible, make this row visible
    if(re){
      return true;
    }
  }

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
StatusController<Tab>::StatusController(std::shared_ptr<Tab> tab)
: tab{tab}
{ }

template<class Tab>
std::shared_ptr<Tab> StatusController<Tab>::get_tab(){
  return tab;
}

template class StatusController<Profiles>;
template class StatusController<Processes>;
template class StatusController<Logs>;
