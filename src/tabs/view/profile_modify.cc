#include "common.h"
#include "../../threads/command_caller.h"
#include "edit_row.h"
#include "profile_modify.h"

#include <apparmor_file_rule.hh>
#include <apparmor_parser.hh>
#include <apparmor_profile.hh>
#include <gtkmm/enums.h>
#include <gtkmm/label.h>
#include <memory>
#include <string>
#include <unordered_set>

// Given a parsed file, return a particular profile corresponding to the "profile_name"
std::unique_ptr<AppArmor::Profile> ProfileModify::find_profile(const AppArmor::Parser &parsed_profile, const std::string &profile_name)
{
  auto profile_list = parsed_profile.getProfileList();
  
  for(auto &profile : profile_list) {
    if(profile.getName() == profile_name)
    {
      return std::make_unique<AppArmor::Profile>(profile);
    }
  }

  return std::unique_ptr<AppArmor::Profile>(nullptr);
}

void ProfileModify::intialize_abstractions(const std::unique_ptr<AppArmor::Profile> &profile)
{
  if(profile != nullptr)
  {
    auto abstractions = profile->getAbstractions();

    for(const std::string &abstraction : abstractions) {
      auto label = std::make_shared<EditRow>(abstraction);
      label->set_halign(Gtk::ALIGN_START);

      m_abstraction_box->pack_end(*label);

      // Insert the widget into the map
      // This ensures it is not deleted before it is drawn
      // Also we will want to retrieve the widget later and delete it, if the abstraction is removed
      abstraction_map.emplace(abstraction, label);
    }
  }
}

void ProfileModify::intialize_file_rules(const std::unique_ptr<AppArmor::Profile> &profile)
{
  if(profile != nullptr)
  {
    auto rules = profile->getFileRules();

    for(AppArmor::FileRule &rule : rules) {
      std::string title = rule.getFilename();
      auto label = std::make_shared<EditRow>(title);
      label->set_halign(Gtk::ALIGN_START);

      m_file_rule_box->pack_end(*label);

      // Insert the widget into the map
      // This ensures it is not deleted before it is drawn
      // Also we will want to retrieve the widget later and delete it, if the abstraction is removed
      auto rule_ptr = std::make_shared<AppArmor::FileRule>(rule);
      file_rule_map.emplace(rule_ptr, label);
    }
  }
}

ProfileModify::ProfileModify(const std::string &profile_name)
  : builder{ Gtk::Builder::create_from_resource("/resources/profile_modify.glade") },
    m_box{ Common::get_widget<Gtk::Box>("m_box", builder) },
    m_title{ Common::get_widget<Gtk::Label>("m_title", builder) },
    m_abstraction_box{ Common::get_widget<Gtk::Box>("m_abstraction_box", builder) },
    m_file_rule_box{ Common::get_widget<Gtk::Box>("m_file_rule_box", builder) }
{
  m_title->set_label(profile_name);

  // Attempt to find and parse this profile
  std::string profile_location = CommandCaller::locate_profile(profile_name) + profile_name;
  std::fstream profile_stream(profile_location);
  AppArmor::Parser parsed(profile_stream);

  // Left justify the children of this box
  m_abstraction_box->set_halign(Gtk::ALIGN_START);
  m_abstraction_box->set_valign(Gtk::ALIGN_START);

  // Find this profile from the file
  std::unique_ptr<AppArmor::Profile> found_profile = find_profile(parsed, profile_name);

  if(found_profile != nullptr) {
    // Initialize the lists of abstractions and file_rules using the parsed profile
    intialize_abstractions(found_profile);
    intialize_file_rules(found_profile);
  }

  this->add(*m_box);
  this->show_all();
}
