#include "common.h"
#include "../../threads/command_caller.h"
#include "edit_row.h"
#include "profile_modify.h"

#include <apparmor_parser.hh>
#include <apparmor_profile.hh>
#include <gtkmm/enums.h>
#include <gtkmm/label.h>
#include <string>
#include <unordered_set>

// Given a parsed file, return a particular profile corresponding to the "profile_name"
std::unordered_set<std::string> ProfileModify::get_abstractions(const AppArmor::Parser &parsed_profile, const std::string &profile_name)
{
  auto profile_list = parsed_profile.getProfileList();
  
  for(auto &profile : profile_list) {
    if(profile.getName() == profile_name)
    {
      return profile.getAbstractions();
    }
  }

  return std::unordered_set<std::string>();
}

void ProfileModify::intialize_abstractions(const AppArmor::Parser &parsed_profile, const std::string &profile_name)
{
  auto abstractions = get_abstractions(parsed_profile, profile_name);

  for(const std::string &abstraction : abstractions) {
    auto label = std::make_shared<EditRow>(abstraction);
    label->set_halign(Gtk::ALIGN_START);

    m_abstraction_box->pack_end(*label);

    // Insert the widget into the map
    // This ensures it is not deleted before it is drawn
    // Also we will want to retrienve the widget later and delete it, if the abstraction is removed
    abstraction_map.emplace(abstraction, label);
  }
}

ProfileModify::ProfileModify(const std::string &profile_name)
  : builder{ Gtk::Builder::create_from_resource("/resources/profile_modify.glade") },
    m_box{ Common::get_widget<Gtk::Box>("m_box", builder) },
    m_title{ Common::get_widget<Gtk::Label>("m_title", builder) },
    m_abstraction_box{ Common::get_widget<Gtk::Box>("m_abstraction_box", builder) }
{
  m_title->set_label(profile_name);

  // Attempt to find and parse this profile
  std::string profile_location = CommandCaller::locate_profile(profile_name) + profile_name;
  std::fstream profile_stream(profile_location);
  AppArmor::Parser parsed_profile(profile_stream);

  // Left justify the children of this box
  m_abstraction_box->set_halign(Gtk::ALIGN_START);
  m_abstraction_box->set_valign(Gtk::ALIGN_START);

  // Initialize the list of abstractions using the parsed profile
  intialize_abstractions(parsed_profile, profile_name);

  this->add(*m_box);
  this->show_all();
}
