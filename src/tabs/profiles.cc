#include "profiles.h"

#include "jsoncpp/json/json.h"

#include <giomm.h>
#include <glibmm.h>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

// add_data_to_record() is based on assumptions about the output of aa-status.
// If those assumptions are incorrect, or aa-status changes, this could crash.
void Profiles::add_data_to_record(const std::string &data)
{
  Json::Value root     = Status::parse_JSON(data);
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

void Profiles::change_status()
{
  auto selection = Status::get_view()->get_selection();

  if(selection->count_selected_rows() == 1) {
    auto row = *selection->get_selected();

    std::string profile_path;
    std::string old_status;
    std::string new_status = Status::get_selection_text();

    row->get_value(0, profile_path);
    row->get_value(1, old_status);

    // Convert the status strings to lower case.
    transform(old_status.begin(), old_status.end(), old_status.begin(), ::tolower);
    transform(new_status.begin(), new_status.end(), new_status.begin(), ::tolower);

    this->profile_status_change_fun(profile_path, old_status, new_status);
  } else {
    Status::set_apply_label_text("Please select a row.");
  }
}

void Profiles::refresh()
{
  uint num_visible = col_record->filter_rows();
  Status::set_status_label_text(" " + std::to_string(num_visible) + " matching profiles");
}

void Profiles::default_change_fun(const std::string &a, const std::string &b, const std::string &c)
{
  std::ignore = this;
  std::ignore = a;
  std::ignore = b;
  std::ignore = c;
  std::cerr << "Warning: No signal handler is defined for changing a profile's status." << std::endl;
}

void Profiles::set_status_change_signal_handler(sigc::slot<void(std::string, std::string, std::string)> change_fun)
{
  profile_status_change_fun = std::move(change_fun);
}

Profiles::Profiles() : col_record{StatusColumnRecord::create(Status::get_view(), Status::get_window(), col_names)}
{
  auto refresh_func = sigc::mem_fun(*this, &Profiles::refresh);
  auto apply_func   = sigc::mem_fun(*this, &Profiles::change_status);
  Status::set_refresh_signal_handler(refresh_func);
  Status::set_apply_signal_handler(apply_func);

  auto filter_fun = sigc::mem_fun(*this, &Profiles::filter);
  col_record->set_visible_func(filter_fun);

  sigc::slot<void(std::string, std::string, std::string)> change_fun = sigc::mem_fun(*this, &Profiles::default_change_fun);
  this->set_status_change_signal_handler(change_fun);

  this->show_all();
}
