#include "profiles_controller.h"
#include "../model/database.h"
#include "../model/profile_adapter.h"
#include "../view/profiles.h"

#include "jsoncpp/json/json.h"
#include "status_controller.h"

#include <cstddef>
#include <giomm.h>
#include <glibmm.h>
#include <gtkmm/treeiter.h>
#include <gtkmm/treepath.h>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

template<class ProfilesTab, class Database, class Adapter> 
bool ProfilesController<ProfilesTab, Database, Adapter>::on_button_event(GdkEventButton* event){
  std::ignore=event;

  handle_profile_selected();
  return false;
}

template<class ProfilesTab, class Database, class Adapter> 
bool ProfilesController<ProfilesTab, Database, Adapter>::on_key_event(GdkEventKey* event){
  std::ignore=event;

  handle_profile_selected();
  return false;
}

template<class ProfilesTab, class Database, class Adapter> 
void ProfilesController<ProfilesTab, Database, Adapter>::handle_profile_selected(){
  // Check if there is any row selected
  auto selection = prof->get_view()->get_selection();
  auto row_selected = selection->count_selected_rows() == 1;

  if(row_selected){
    Gtk::TreePath path = selection->get_selected_rows()[0]; 
    Gtk::TreeRow row = adapter.get_col_record()->get_row(path);

    std::string profile_name;
    row.get_value(0, profile_name);
    
    auto entry_pair = adapter.get_data(profile_name); 
    auto entry = entry_pair.first;
    bool found = entry_pair.second;

    if(found){
      uint num_log  = adapter.get_number_logs(profile_name);
      uint num_proc = adapter.get_number_processes(profile_name);
      prof->set_profile_info(std::to_string(num_log) + " related logs", "Not implemented yet!", std::to_string(num_proc) + " running processes");
      prof->show_profile_info();
    }
  } else {
    prof -> hide_profile_info();
  }
}

// add_data_to_record() is based on assumptions about the output of aa-status.
// If those assumptions are incorrect, or aa-status changes, this could crash.
template<class ProfilesTab, class Database, class Adapter> 
void ProfilesController<ProfilesTab, Database, Adapter>::add_data_to_record(const std::string &data)
{
  Json::Value root     = StatusController<ProfilesTab>::parse_JSON(data);
  Json::Value profiles = root["profiles"];

  for(auto profile = profiles.begin(); profile != profiles.end(); profile++) {
    auto profile_name = profile.key().asString();
    auto status       = profiles.get(profile_name, UNKNOWN_STR).asString();
    adapter.put_data(profile_name, status);
  }

  refresh();
}

template<class ProfilesTab, class Database, class Adapter> 
void ProfilesController<ProfilesTab, Database, Adapter>::refresh()
{
  uint num_visible = adapter.get_col_record()->filter_rows();
  prof->set_status_label_text(" " + std::to_string(num_visible) + " matching profiles");
  handle_profile_selected();
}

template<class ProfilesTab, class Database, class Adapter> 
void ProfilesController<ProfilesTab, Database, Adapter>::set_apply_label_text(const std::string &str){
  prof->set_apply_label_text(str);
}

template<class ProfilesTab, class Database, class Adapter> 
ProfilesController<ProfilesTab, Database, Adapter>::ProfilesController(std::shared_ptr<Database> database) 
  : StatusController<ProfilesTab>(),
    prof{StatusController<ProfilesTab>::get_tab()},
    adapter(database, prof->get_view(), prof->get_window())
{

  auto func = sigc::mem_fun(*this, &ProfilesController<ProfilesTab, Database, Adapter>::refresh);
  prof->set_refresh_signal_handler(func);

  auto filter_fun = sigc::mem_fun(*this, &ProfilesController::filter);
  adapter.get_col_record()->set_visible_func(filter_fun);

  // When a key/button is pressed or released, check if the selection has changed
  auto button_event_fun = sigc::mem_fun(*this, &ProfilesController::on_button_event);
  prof->get_view()->signal_button_release_event().connect(button_event_fun, true);

  auto key_event_fun = sigc::mem_fun(*this, &ProfilesController::on_key_event);
  prof->get_view()->signal_key_release_event().connect(key_event_fun, true);

  prof->get_view()->set_activate_on_single_click(true);
}

// Used to avoid linker errors
// For more information, see: https://isocpp.org/wiki/faq/templates#class-templates
template class ProfilesController<Profiles, Database, ProfileAdapter<Database>>;