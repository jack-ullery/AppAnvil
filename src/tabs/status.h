#ifndef GTKMM_EXAMPLE_STATUS_H
#define GTKMM_EXAMPLE_STATUS_H

#include "jsoncpp/json/json.h"
#include <sstream>

class Status
{
  public:
    static std::string get_status_str();
    static Json::Value get_status_JSON();

  private:
    static Json::Value parse_JSON(const std::string& raw_json);
};

#endif // GTKMM_EXAMPLE_STATUS_H
