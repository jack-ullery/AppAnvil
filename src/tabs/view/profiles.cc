#include "profiles.h"
#include "../model/status_column_record.h"

#include "jsoncpp/json/json.h"
#include "status.h"

#include <giomm.h>
#include <glibmm.h>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

template<typename T_Widget> std::shared_ptr<T_Widget> Profiles::get_widget_shared(Glib::ustring name, const Glib::RefPtr<Gtk::Builder> &builder)
{
  T_Widget *raw_addr = nullptr;
  builder->get_widget<T_Widget>(name, raw_addr);
  return std::shared_ptr<T_Widget>(raw_addr);
}

void Profiles::change_status()
{
  auto selection = Status::get_view()->get_selection();

  if(selection->count_selected_rows() == 1) {
    auto row = *selection->get_selected();

    std::string profile_path;
    std::string old_status;
    // Get the status that we intend to switch to
    std::string new_status = p_status_selection->get_active_text();

    row->get_value(0, profile_path);
    row->get_value(1, old_status);

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

void Profiles::set_apply_label_text(const std::string &str) {
  p_apply_info_text->set_text(str);
}

Profiles::Profiles()
    : Status("/resources/profile.glade"),
      builder{Status::get_builder()},
      p_status_selection{get_widget_shared<Gtk::ComboBoxText>("p_status_selection", builder)},
      p_apply_button{get_widget_shared<Gtk::Button>("p_apply_button", builder)},
      p_apply_info_text{get_widget_shared<Gtk::Label>("p_apply_info_text", builder)}
{
  // Set the function to be called when the apply button is pressed
  auto change_fun = sigc::mem_fun(*this, &Profiles::change_status);
  p_apply_button->signal_clicked().connect(change_fun, true);

  this->show_all();
}