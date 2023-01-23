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

void ProfileModify::intialize_abstractions(const AppArmor::Profile &profile)
{
  auto abstractions = profile.getAbstractions();

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

void ProfileModify::intialize_file_rules(const AppArmor::Profile &profile)
{
  auto rules = profile.getFileRules();

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

ProfileModify::ProfileModify(const AppArmor::Profile &profile)
  : builder{ Gtk::Builder::create_from_resource("/resources/profile_modify.glade") },
    m_box{ Common::get_widget<Gtk::Box>("m_box", builder) },
    m_title{ Common::get_widget<Gtk::Label>("m_title", builder) },
    m_abstraction_box{ Common::get_widget<Gtk::Box>("m_abstraction_box", builder) },
    m_file_rule_box{ Common::get_widget<Gtk::Box>("m_file_rule_box", builder) }
{
  const auto &profile_name = profile.name();
  m_title->set_label(profile_name);

  // Left justify the children of this box
  m_abstraction_box->set_halign(Gtk::ALIGN_START);
  m_abstraction_box->set_valign(Gtk::ALIGN_START);

  // Initialize the lists of abstractions and file_rules using the parsed profile
  intialize_abstractions(profile);
  intialize_file_rules(profile);

  this->add(*m_box);
  this->show_all();
}
