#include "profiles_controller.h"
#include "../model/status_column_record.h"
#include "../view/profiles.h"

#include "jsoncpp/json/json.h"
#include "status_controller.h"

#include <giomm.h>
#include <glibmm.h>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

// add_data_to_record() is based on assumptions about the output of aa-status.
// If those assumptions are incorrect, or aa-status changes, this could crash.
template<class ProfilesTab, class ColumnRecord> 
void ProfilesController<ProfilesTab, ColumnRecord>::add_data_to_record(const std::string &data)
{
  Json::Value root     = StatusController<ProfilesTab>::parse_JSON(data);
  Json::Value profiles = root["profiles"];

  col_record->clear();

  for(auto profile = profiles.begin(); profile != profiles.end(); profile++) {
    std::string key = profile.key().asString();
    auto row        = col_record->new_row();
    row->set_value(0, key);
    row->set_value(1, profiles.get(key, UNKNOWN_STATUS).asString());
  }

  col_record->reselect_rows();
  refresh();
}

template<class ProfilesTab, class ColumnRecord> 
void ProfilesController<ProfilesTab, ColumnRecord>::refresh()
{
  uint num_visible = col_record->filter_rows();
  prof->set_status_label_text(" " + std::to_string(num_visible) + " matching profiles");
}


template<class ProfilesTab, class ColumnRecord> 
ProfilesController<ProfilesTab, ColumnRecord>::ProfilesController() 
: prof{StatusController<ProfilesTab>::get_tab()}, 
  col_record{ColumnRecord::create(prof->get_view(), prof->get_window(), col_names)}
{
  auto func = sigc::mem_fun(*this, &ProfilesController<ProfilesTab, ColumnRecord>::refresh);
  prof->set_refresh_signal_handler(func);

  auto filter_fun = sigc::mem_fun(*this, &ProfilesController::filter);
  col_record->set_visible_func(filter_fun);
}

// Used to avoid linker errors
// For more information, see: https://isocpp.org/wiki/faq/templates#class-templates
template class ProfilesController<Profiles<StatusColumnRecord>, StatusColumnRecord>;