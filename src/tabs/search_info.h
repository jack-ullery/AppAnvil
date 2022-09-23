#ifndef TABS_SEARCH_INFO_H
#define TABS_SEARCH_INFO_H

#include <string>

struct SearchInfo
{
  std::string filter_rule;
  bool use_regex;
  bool match_case;
  bool whole_word;

  SearchInfo(const std::string &_filter_rule, const bool &_use_regex, const bool &_match_case, const bool &_whole_word)
    : filter_rule{ _filter_rule },
      use_regex{ _use_regex },
      match_case{ _match_case },
      whole_word{ _whole_word }
  {
  }
};

#endif // TABS_SEARCH_INFO_H