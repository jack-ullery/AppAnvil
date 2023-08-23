#include "profile_modify.h"
#include "../../threads/command_caller.h"
#include "common.h"

#include <exception>
#include <gtkmm/button.h>
#include <gtkmm/enums.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>
#include <iostream>
#include <libappanvil/apparmor_parser.hh>
#include <memory>
#include <string>
#include <unordered_set>

template<class AppArmorParser>
std::shared_ptr<Gtk::TreeView> ProfileModifyImpl<AppArmorParser>::get_abstraction_view()
{
  return m_abstraction_view;
}

template<class AppArmorParser>
std::shared_ptr<Gtk::TreeView> ProfileModifyImpl<AppArmorParser>::get_file_rule_view()
{
  return m_file_rule_view;
}

template<class AppArmorParser>
void ProfileModifyImpl<AppArmorParser>::update_profile_text()
{
  const std::string profile_data = parser->operator std::string();

  m_profile_text->get_buffer()->set_text(profile_data);
}

template<class AppArmorParser>
ProfileModifyImpl<AppArmorParser>::ProfileModifyImpl(std::shared_ptr<AppArmorParser> parser, std::shared_ptr<AppArmor::Profile> profile)
  : builder{ Gtk::Builder::create_from_resource("/resources/profile_modify.glade") },
    m_box{ Common::get_widget<Gtk::Box>("m_box", builder) },
    m_title_1{ Common::get_widget<Gtk::Label>("m_title_1", builder) },
    m_title_2{ Common::get_widget<Gtk::Label>("m_title_2", builder) },
    m_title_3{ Common::get_widget<Gtk::Label>("m_title_3", builder) },
    m_abstraction_view{ Common::get_widget<Gtk::TreeView>("m_abstraction_view", builder) },
    m_file_rule_view{ Common::get_widget<Gtk::TreeView>("m_file_rule_view", builder) },
    m_profile_text{ Common::get_widget<Gtk::TextView>("m_profile_text", builder) },
    parser{ parser }
{
  const auto &profile_name = profile->name();
  m_title_1->set_label(profile_name);
  m_title_2->set_label(profile_name);
  m_title_3->set_label(profile_name);

  update_profile_text();

  this->add(*m_box);
  this->show_all();
}

template class ProfileModifyImpl<AppArmor::Parser>;