#include "profiles.h"
#include "../model/status_column_record.h"

#include "jsoncpp/json/json.h"

#include <giomm.h>
#include <glibmm.h>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

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

Profiles::Profiles()
{
  auto apply_func   = sigc::mem_fun(*this, &Profiles::change_status);
  Status::set_apply_signal_handler(apply_func);

  sigc::slot<void(std::string, std::string, std::string)> change_fun = sigc::mem_fun(*this, &Profiles::default_change_fun);
  this->set_status_change_signal_handler(change_fun);

  this->show_all();
}