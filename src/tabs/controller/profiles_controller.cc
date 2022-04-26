#include "profiles_controller.h"
#include "../model/status_column_record.h"

#include "jsoncpp/json/json.h"

#include <giomm.h>
#include <glibmm.h>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

// add_data_to_record() is based on assumptions about the output of aa-status.
// If those assumptions are incorrect, or aa-status changes, this could crash.
template<class ColumnRecord> 
void ProfilesController<ColumnRecord>::add_data_to_record(const std::string &data)
{
  Json::Value root     = StatusController::parse_JSON(data);
  Json::Value profiles = root["profiles"];

  col_record->clear();

  for(auto prof = profiles.begin(); prof != profiles.end(); prof++) {
    std::string key = prof.key().asString();
    auto row        = col_record->new_row();
    row->set_value(0, key);
    row->set_value(1, profiles.get(key, UNKNOWN_STATUS).asString());
  }

  col_record->reselect_rows();
  refresh();
}

template<class ColumnRecord> 
ProfilesController<ColumnRecord>::ProfilesController() : col_record{ColumnRecord::create(Status::get_view(), Status::get_window(), col_names)}
{
  auto filter_fun = sigc::mem_fun(*this, &ProfilesController::filter);
  col_record->set_visible_func(filter_fun);
}

// Used to avoid linker errors
// For more information, see: https://isocpp.org/wiki/faq/templates#class-templates
template class ProfilesController<StatusColumnRecord>;