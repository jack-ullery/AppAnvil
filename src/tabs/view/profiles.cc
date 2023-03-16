#include "profiles.h"
#include "../model/status_column_record.h"
#include "common.h"

#include "profile_loader.h"
#include "status.h"

#include <giomm.h>
#include <glibmm.h>
#include <gtkmm/box.h>
#include <iostream>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

void Profiles::change_status()
{
  auto selection = Status::get_view()->get_selection();

  if (selection->count_selected_rows() == 1) {
    auto row = *selection->get_selected();

    std::string profile_path;
    std::string old_status;
    // Get the status that we intend to switch to
    std::string new_status = p_status_selection->get_active_text();

    row->get_value(1, profile_path);
    row->get_value(2, old_status);

    // Convert the status strings to lower case.
    transform(old_status.begin(), old_status.end(), old_status.begin(), ::tolower);
    transform(new_status.begin(), new_status.end(), new_status.begin(), ::tolower);

    this->profile_status_change_fun(profile_path, old_status, new_status);
  } else {
    p_apply_info_text->set_text("Please select a row.");
  }
}

void Profiles::set_status_change_signal_handler(sigc::slot<void(std::string, std::string, std::string)> change_fun)
{
  profile_status_change_fun = std::move(change_fun);
}

void Profiles::set_apply_label_text(const std::string &str)
{
  p_apply_info_text->set_text(str);
}

void Profiles::set_profile_info(const std::string &num_logs, const std::string &num_perms, const std::string &num_procs)
{
  p_num_log_label->set_text(num_logs);
  p_num_proc_label->set_text(num_procs);
  p_num_perm_label->set_text(num_perms);
}

void Profiles::show_profile_info()
{
  p_profile_info->show_all();
  p_change_state_toggle->set_sensitive(true);
}

void Profiles::hide_profile_info()
{
  p_profile_info->hide();
  p_state_selection_box->hide();

  p_change_state_toggle->set_active(false);
  p_change_state_toggle->set_sensitive(false);
}

void Profiles::handle_change_state_toggle()
{
  bool active = p_change_state_toggle->get_active();

  if (active) {
    // We cannot have two toggles active at the same time, and the current version of GTKmm does not seem to have button groups
    p_load_profile_toggle->set_active(false);
    p_state_selection_box->show_all();
  } else {
    // Hide the state selection, when not in use
    p_state_selection_box->hide();
  }
}

void Profiles::handle_load_profile_toggle()
{
  bool active = p_load_profile_toggle->get_active();

  if (active) {
    // We cannot have two toggles active at the same time, and the current version of GTKmm does not seem to have button groups
    p_change_state_toggle->set_active(false);
    p_stack->set_visible_child("loadProfile");
  } else {
    p_stack->set_visible_child("viewProfile");
  }
}

Profiles::Profiles()
  : Status("/resources/profile.glade"),
    builder{ Status::get_builder() },
    p_change_state_toggle{ Common::get_widget<Gtk::ToggleButton>("p_change_state_toggle", builder) },
    p_load_profile_toggle{ Common::get_widget<Gtk::ToggleButton>("p_load_profile_toggle", builder) },
    p_stack{ Common::get_widget<Gtk::Stack>("p_stack", builder) },
    p_state_selection_box{ Common::get_widget<Gtk::Box>("p_state_selection_box", builder) },
    p_status_selection{ Common::get_widget<Gtk::ComboBoxText>("p_status_selection", builder) },
    p_apply_button{ Common::get_widget<Gtk::Button>("p_apply_button", builder) },
    p_apply_info_text{ Common::get_widget<Gtk::Label>("p_apply_info_text", builder) },
    p_profile_info{ Common::get_widget<Gtk::Box>("p_profile_info", builder) },
    p_num_log_label{ Common::get_widget<Gtk::Label>("p_num_log_label", builder) },
    p_num_proc_label{ Common::get_widget<Gtk::Label>("p_num_proc_label", builder) },
    p_num_perm_label{ Common::get_widget<Gtk::Label>("p_num_perm_label", builder) },
    loader{ new ProfileLoader() }
{
  // Add tabs to the stack pane
  p_stack->add(*loader, "loadProfile");

  // Configure the button used for changing a profiles confinement
  auto change_state_toggle_fun = sigc::mem_fun(*this, &Profiles::handle_change_state_toggle);
  p_change_state_toggle->signal_toggled().connect(change_state_toggle_fun);

  // Configure the button used for loading a profile
  auto load_profile_toggle_fun = sigc::mem_fun(*this, &Profiles::handle_load_profile_toggle);
  p_load_profile_toggle->signal_toggled().connect(load_profile_toggle_fun);

  // Set the function to be called when the apply button is pressed
  auto change_fun = sigc::mem_fun(*this, &Profiles::change_status);
  p_apply_button->signal_clicked().connect(change_fun);

  this->show_all();

  this->hide_profile_info();
  p_state_selection_box->hide();
}