#ifndef TABS_SEARCH_INFO_H
#define TABS_SEARCH_INFO_H

#include <string>

struct SearchInfo {
  const std::string &filter_rule;
  const bool &use_regex;
  const bool &match_case;
  const bool &whole_word;

  SearchInfo(const std::string &_filter_rule, const bool &_use_regex, const bool &_match_case, const bool &_whole_word) 
  : filter_rule{_filter_rule},
    use_regex  {_use_regex},
    match_case {_match_case},
    whole_word {_whole_word}
  {
  }
};

#endif // TABS_SEARCH_INFO_H