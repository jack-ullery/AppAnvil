#include "profiles_controller.h"
#include "../model/status_column_record.h"
#include "../model/database.h"
#include "../view/profiles.h"

#include "jsoncpp/json/json.h"
#include "status_controller.h"

#include <cstddef>
#include <giomm.h>
#include <glibmm.h>
#include <gtkmm/treeiter.h>
#include <gtkmm/treepath.h>
#include <string>
#include <tuple>
#include <vector>

template<class ProfilesTab, class ColumnRecord, class Database> 
void ProfilesController<ProfilesTab, ColumnRecord, Database>::on_row_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column){
  // Ignore unused parameters
  std::ignore=path;
  std::ignore=column;

  handle_profile_selected();
}

template<class ProfilesTab, class ColumnRecord, class Database> 
void ProfilesController<ProfilesTab, ColumnRecord, Database>::handle_profile_selected(){
  // Check if there is any row selected
  auto selection = prof->get_view()->get_selection();
  auto row_selected = selection->count_selected_rows() == 1;

  if(row_selected){
    Gtk::TreePath path = selection->get_selected_rows()[0]; 
    Gtk::TreeRow row = col_record->get_row(path);

    std::string profile_name;
    row.get_value(0, profile_name);
    
    ProfileTableEntry entry = database->get_profile_data(profile_name); 

    prof->set_profile_info(entry.status, "Not implemented yet!", "Not implemented yet!");
    prof->show_profile_info();
  }
}

// add_data_to_record() is based on assumptions about the output of aa-status.
// If those assumptions are incorrect, or aa-status changes, this could crash.
template<class ProfilesTab, class ColumnRecord, class Database> 
void ProfilesController<ProfilesTab, ColumnRecord, Database>::add_data_to_record(const std::string &data)
{
  Json::Value root     = StatusController<ProfilesTab>::parse_JSON(data);
  Json::Value profiles = root["profiles"];

  for(auto profile = profiles.begin(); profile != profiles.end(); profile++) {
    auto profile_name = profile.key().asString();
    auto status       = profiles.get(profile_name, UNKNOWN_STATUS).asString();
    database->put_profile_data(profile_name, status);
  }

  refresh();
}

template<class ProfilesTab, class ColumnRecord, class Database> 
void ProfilesController<ProfilesTab, ColumnRecord, Database>::refresh()
{
  uint num_visible = col_record->filter_rows();
  prof->set_status_label_text(" " + std::to_string(num_visible) + " matching profiles");
  handle_profile_selected();
}

template<class ProfilesTab, class ColumnRecord, class Database> 
void ProfilesController<ProfilesTab, ColumnRecord, Database>::set_apply_label_text(const std::string &str){
  prof->set_apply_label_text(str);
}

template<class ProfilesTab, class ColumnRecord, class Database> 
ProfilesController<ProfilesTab, ColumnRecord, Database>::ProfilesController(std::shared_ptr<Database> database) 
: database{database},
  prof{StatusController<ProfilesTab>::get_tab()}, 
  col_record{ColumnRecord::create(prof->get_view(), prof->get_window(), col_names)}
{
  database->register_profile_column_record(col_record);

  auto func = sigc::mem_fun(*this, &ProfilesController<ProfilesTab, ColumnRecord, Database>::refresh);
  prof->set_refresh_signal_handler(func);

  auto filter_fun = sigc::mem_fun(*this, &ProfilesController::filter);
  col_record->set_visible_func(filter_fun);

  auto row_activate_fun = sigc::mem_fun(*this, &ProfilesController::on_row_activated);
  prof->get_view()->signal_row_activated().connect(row_activate_fun, true);
  prof->get_view()->set_activate_on_single_click(true);
}

// Used to avoid linker errors
// For more information, see: https://isocpp.org/wiki/faq/templates#class-templates
template class ProfilesController<Profiles, StatusColumnRecord, Database>;