#include "profiles_controller.h"
#include "../model/database.h"
#include "../model/profile_adapter.h"
#include "../view/profiles.h"
#include "status_controller.h"

#include <cstddef>
#include <giomm.h>
#include <glibmm.h>
#include <gtkmm/treeiter.h>
#include <gtkmm/treepath.h>
#include <iostream>
#include <json/json.h>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

template<class ProfilesTab, class Database, class Adapter>
bool ProfilesController<ProfilesTab, Database, Adapter>::on_button_event(GdkEventButton *event)
{
  std::ignore = event;

  handle_profile_selected();
  return false;
}

template<class ProfilesTab, class Database, class Adapter>
bool ProfilesController<ProfilesTab, Database, Adapter>::on_key_event(GdkEventKey *event)
{
  std::ignore = event;

  handle_profile_selected();
  return false;
}

template<class ProfilesTab, class Database, class Adapter>
void ProfilesController<ProfilesTab, Database, Adapter>::handle_profile_selected()
{
  // Check if there is any row selected
  auto selection    = prof->get_view()->get_selection();
  auto row_selected = selection->count_selected_rows() == 1;

  if (row_selected) {
    Gtk::TreePath path = selection->get_selected_rows()[0];
    Gtk::TreeRow row   = adapter.get_col_record()->get_row(path);

    ProfileTableEntry entry;
    row.get_value(0, entry);

    uint num_log  = adapter.get_number_logs(entry.profile_name);
    uint num_proc = adapter.get_number_processes(entry.profile_name);
    prof->set_profile_info(std::to_string(num_log) + " related logs", std::to_string(num_proc) + " running processes");

    prof->show_profile_info();
  } else {
    prof->hide_profile_info();
  }
}

template<class ProfilesTab, class Database, class Adapter>
void ProfilesController<ProfilesTab, Database, Adapter>::change_status(const std::string &path)
{
  auto iter = adapter.get_col_record()->get_iter(path);

  if (iter != nullptr) {
    const auto &row = *iter;

    ProfileTableEntry entry;
    std::string profile;
    std::string new_status;

    row->get_value(0, entry);
    row->get_value(1, profile);
    row->get_value(2, new_status);

    std::string old_status = entry.status;

    // Convert the status strings to lower case.
    transform(old_status.begin(), old_status.end(), old_status.begin(), ::tolower);
    transform(new_status.begin(), new_status.end(), new_status.begin(), ::tolower);

    std::string profile_path = prof->find_path(profile);

    this->profile_status_change_fun(profile_path, old_status, new_status);

    // Update the entry and refresh the table
    entry.status = new_status;
    row->set_value(0, entry);
    refresh();

  } else {
    std::cerr << "Error: Could not retrieve row when changing status" << std::endl;
  }
}

template<class ProfilesTab, class Database, class Adapter>
void ProfilesController<ProfilesTab, Database, Adapter>::set_status_change_signal_handler(
  sigc::slot<void(std::string, std::string, std::string)> change_fun)
{
  profile_status_change_fun = std::move(change_fun);
}

// add_data_to_record() is based on assumptions about the output of aa-status.
// If those assumptions are incorrect, or aa-status changes, this could crash.
template<class ProfilesTab, class Database, class Adapter>
void ProfilesController<ProfilesTab, Database, Adapter>::add_data_to_record(const std::string &data)
{
  Json::Value root     = StatusController<ProfilesTab>::parse_JSON(data);
  Json::Value profiles = root["profiles"];

  // Parse each object in the "profiles" field of the json,
  //   and attempt to add each tuple to this db
  bool changed = false;
  for (auto profile = profiles.begin(); profile != profiles.end(); profile++) {
    auto profile_name = profile.key().asString();
    auto status       = profiles.get(profile_name, UNKNOWN_STR).asString();
    changed |= adapter.put_data(profile_name, status);
  }

  // Refilter the table, but only if tuples were added
  if(changed)
  {
    refresh();
  }
}

template<class ProfilesTab, class Database, class Adapter>
void ProfilesController<ProfilesTab, Database, Adapter>::refresh()
{
  // Update the label in the profiles section
  std::stringstream label;
  label << " " << adapter.get_col_record()->filter_rows() << " matching profiles";
  prof->set_status_label_text(label.str());

  // Check if any row was selected/unselected
  handle_profile_selected();
}

template<class ProfilesTab, class Database, class Adapter>
ProfilesController<ProfilesTab, Database, Adapter>::ProfilesController(std::shared_ptr<Database> database)
  : StatusController<ProfilesTab>(),
    prof{ StatusController<ProfilesTab>::get_tab() },
    adapter(database, prof->get_view())
{
  auto func = sigc::mem_fun(*this, &ProfilesController<ProfilesTab, Database, Adapter>::refresh);
  prof->set_refresh_signal_handler(func);

  auto filter_fun = sigc::mem_fun(*this, &ProfilesController::filter);
  adapter.get_col_record()->set_visible_func(filter_fun);

  // When the profile status dropdown is changed, attempt to change the status
  auto change_fun = sigc::mem_fun(*this, &ProfilesController::change_status);
  adapter.get_col_record()->set_change_func(change_fun);

  // When a key/button is pressed or released, check if the selection has changed
  auto button_event_fun = sigc::mem_fun(*this, &ProfilesController::on_button_event);
  prof->get_view()->signal_button_release_event().connect(button_event_fun, true);

  auto key_event_fun = sigc::mem_fun(*this, &ProfilesController::on_key_event);
  prof->get_view()->signal_key_release_event().connect(key_event_fun, true);
}

// Used to avoid linker errors
// For more information, see: https://isocpp.org/wiki/faq/templates#class-templates
template class ProfilesController<Profiles, Database, ProfileAdapter<Database>>;