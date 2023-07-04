#include "profiles.h"
#include "../../threads/command_caller.h"
#include "../entries.h"
#include "../model/status_column_record.h"
#include "common.h"
#include "profile_loader.h"
#include "profile_modify.h"
#include "status.h"

#include <giomm.h>
#include <glibmm.h>
#include <gtkmm/box.h>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

void Profiles::set_profile_info(const std::string &num_logs, const std::string &num_procs)
{
  p_num_log_label->set_text(num_logs);
  p_num_proc_label->set_text(num_procs);
}

std::string Profiles::find_path(const std::string &profile_name)
{
  auto pair = profile_map.find(profile_name);
  if (pair != profile_map.end()) {
    return pair->second.first.getPath();
  }

  return "";
}

void Profiles::show_profile_info()
{
  p_profile_info->show_all();

  // See if their is a profile selected
  auto selection = Status::get_view()->get_selection();
  if (selection->count_selected_rows() == 1) {
    // Get the profile from the selected row
    std::string profile_name;
    auto row = *selection->get_selected();
    row->get_value(1, profile_name);

    // If the profile has been parsed, allow us to modify it
    if (profile_map.contains(profile_name)) {
      p_modify_profile_toggle->set_sensitive(true);
    } else {
      p_modify_profile_toggle->set_sensitive(false);
    }
  }
}

void Profiles::hide_profile_info()
{
  p_profile_info->hide();

  p_modify_profile_toggle->set_active(false);
  p_modify_profile_toggle->set_sensitive(false);
}

void Profiles::handle_load_profile_toggle()
{
  bool active = p_load_profile_toggle->get_active();

  if (active) {
    // We cannot have two toggles active at the same time, and the current version of GTKmm does not seem to have button groups
    p_modify_profile_toggle->set_active(false);
    p_stack->set_visible_child("loadProfile");
  } else {
    p_stack->set_visible_child("viewProfile");
  }
}

void Profiles::handle_modify_profile_toggle()
{
  bool active = p_modify_profile_toggle->get_active();

  if (active) {
    // We cannot have two toggles active at the same time, and the current version of GTKmm does not seem to have button groups
    p_load_profile_toggle->set_active(false);

    auto selection = Status::get_view()->get_selection();

    if (selection->count_selected_rows() == 1) {
      auto row = *selection->get_selected();

      std::string profile_name;
      row->get_value(1, profile_name);

      if (!modifiers.contains(profile_name)) {
        auto profile_pair = profile_map.find(profile_name);

        if (profile_pair != profile_map.end()) {
          CommandCaller::parser_profile_pair tuple = profile_pair->second;
          auto parser                              = std::make_shared<AppArmor::Parser>(tuple.first);
          auto profile                             = std::make_shared<AppArmor::Profile>(tuple.second);
          auto controller                          = std::make_shared<ProfileModifyController>(parser, profile);
          auto profile_modify                      = controller->get_profile_modify();

          modifiers.insert_or_assign(profile_name, controller);
          p_stack->add(*profile_modify, "modifyProfile_" + profile_name);
        }
      }

      p_stack->set_visible_child("modifyProfile_" + profile_name);
    }
  } else {
    p_stack->set_visible_child("viewProfile");
  }
}

Profiles::Profiles()
  : Status("/resources/profile.glade"),
    builder{ Status::get_builder() },
    p_load_profile_toggle{ Common::get_widget<Gtk::ToggleButton>("p_load_profile_toggle", builder) },
    p_modify_profile_toggle{ Common::get_widget<Gtk::ToggleButton>("p_modify_profile_toggle", builder) },
    p_stack{ Common::get_widget<Gtk::Stack>("p_stack", builder) },
    p_profile_info{ Common::get_widget<Gtk::Box>("p_profile_info", builder) },
    p_num_log_label{ Common::get_widget<Gtk::Label>("p_num_log_label", builder) },
    p_num_proc_label{ Common::get_widget<Gtk::Label>("p_num_proc_label", builder) },
    loader{ std::make_unique<ProfileLoader>() },
    profile_map{ CommandCaller::get_profiles() }
{
  // Add tabs to the stack pane
  p_stack->add(*loader, "loadProfile");

  // Configure the button used for loading a profile
  auto load_profile_toggle_fun = sigc::mem_fun(*this, &Profiles::handle_load_profile_toggle);
  p_load_profile_toggle->signal_toggled().connect(load_profile_toggle_fun);

  // Configure the button used for loading a profile
  auto modify_profile_toggle_fun = sigc::mem_fun(*this, &Profiles::handle_modify_profile_toggle);
  p_modify_profile_toggle->signal_toggled().connect(modify_profile_toggle_fun);

  this->show_all();

  this->hide_profile_info();
}