#include "jsoncpp/json/json.h"
#include "profiles.h"

#include <giomm.h>
#include <glibmm.h>
#include <iostream>
#include <string>
#include <vector>

// refresh() is based on assumptions about the output of aa-status.
// If those assumptions are incorrect, or aa-status changes, this could crash.
void Profiles::refresh(){
  Json::Value root = Status::get_status_JSON();
  Json::Value profiles = root["profiles"];

  int num_found = 0;

  col_record->clear();
  for(auto prof = profiles.begin(); prof != profiles.end(); prof++){
    std::string key = prof.key().asString();
    if(filter(key)){
      auto row = col_record->new_row();
      col_record->set_row_data(row, 0, key);
      col_record->set_row_data(row, 1, profiles.get(key, UNKNOWN_STATUS).asString());
      num_found++;
    }
  }

  Status::set_status_label_text(" " + std::to_string(num_found) + " matching profiles");
}

bool Profiles::execute_change(const std::string& profile, const std::string& status){
  std::string new_status; // stores console command based on the potential new status
  std::string selection = Status::get_selection_text(); // gets the active selection in the dropdown menu

  if (selection == "Enforce" && status != "enforce") {
    new_status = "aa-enforce";
  } else if (selection == "Complain" && status != "complain") {
    new_status = "aa-complain";
  } else if (selection == "Disable" && status != "disabled") {
    new_status = "aa-disable";
  } else {
    selection[0] = tolower(selection[0]); // for  a e s t h e t i c s 
    Status::set_apply_label_text("'" + profile + "' already set to " + selection + ".");
    return true;
  }

  // command to change the profile to the provided status
  std::vector<std::string> args = {"sudo", new_status, profile};

  std::string child_output;
  std::string child_error;
  int exit_status = 0;

  // executed in commandline, copied from status.cc 
  Glib::spawn_sync("/usr/sbin/", args, Glib::SpawnFlags::SPAWN_DEFAULT, {}, &child_output, &child_error, &exit_status);

  if(exit_status != 0){
    std::cout << "Error calling '"<< args[0] <<"'. " << child_error << std::endl;
    child_output = "{\"processes\": {}, \"profiles\": {}";
  } else {
    Status::set_apply_label_text(" Changed '" + profile + "' from " + status + " to " + new_status.erase(0,3));
  }

  return true;
}

void Profiles::change_status(){
  const auto view_obj = Status::get_view();
  const auto spinner_obj = Status::get_spinner();

  spinner_obj->start();

  const auto row_obj = view_obj->get_selection();
  const auto iter = row_obj->get_selected();
  const auto row = *iter;
  const auto path = col_record->get_row_data(row, 0);
  const auto status = col_record->get_row_data(row, 1);
  
  if(!execute_change(path, status)) { // this is useless at the moment (always true)
    std::cout << "Error changing the status" << std::endl;
  }

  refresh();

  spinner_obj->stop();
}

Profiles::Profiles()
: col_record{StatusColumnRecord::create(Status::get_view(), col_names)}
{
  auto func = sigc::mem_fun(*this, &Profiles::on_search_changed);
  auto func_2 = sigc::mem_fun(*this, &Profiles::on_apply_button_pressed);
  Status::set_refresh_signal_handler(func);
  Status::set_apply_signal_handler(func_2);

  this->show_all();
}

void Profiles::on_search_changed(){
  refresh();
}

void Profiles::on_apply_button_pressed(){
  change_status();
}