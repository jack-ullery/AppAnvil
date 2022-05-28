#include "profiles_controller.h"
#include "../model/status_column_record.h"
#include "../view/profiles.h"

#include "jsoncpp/json/json.h"
#include "status_controller.h"

#include <cstddef>
#include <giomm.h>
#include <glibmm.h>
#include <string>
#include <tuple>
#include <vector>

template<class ProfilesTab, class ColumnRecord> 
void ProfilesController<ProfilesTab, ColumnRecord>::on_row_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column){
  // Ignore unused parameters
  std::ignore=path;
  std::ignore=column;

  handle_profile_selected();
}

template<class ProfilesTab, class ColumnRecord> 
void ProfilesController<ProfilesTab, ColumnRecord>::handle_profile_selected(){
  // Check if there is any row selected
  auto row_selected = prof->get_view()->get_selection()->count_selected_rows() == 1;

  if(row_selected){
    prof->show_profile_info();
    prof->set_selected_profile_information("Not implemented yet!", "Not implemented yet!", "Not implemented yet!");
  }
}

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
  handle_profile_selected();
}

template<class ProfilesTab, class ColumnRecord> 
void ProfilesController<ProfilesTab, ColumnRecord>::set_apply_label_text(const std::string &str){
  prof->set_apply_label_text(str);
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

  auto row_activate_fun = sigc::mem_fun(*this, &ProfilesController::on_row_activated);
  prof->get_view()->signal_row_activated().connect(row_activate_fun, true);
  prof->get_view()->set_activate_on_single_click(true);
}

// Used to avoid linker errors
// For more information, see: https://isocpp.org/wiki/faq/templates#class-templates
template class ProfilesController<Profiles, StatusColumnRecord>;