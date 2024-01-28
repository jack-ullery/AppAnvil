#include "profile_modify.h"
#include "../../threads/command_caller.h"
#include "common.h"
#include "scrolled_view.h"

#include <exception>
#include <gtkmm/button.h>
#include <gtkmm/enums.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>
#include <libappanvil/apparmor_parser.hh>
#include <memory>
#include <string>
#include <unordered_set>

template<class AppArmorParser>
std::shared_ptr<Gtk::TreeView> ProfileModifyImpl<AppArmorParser>::get_abstraction_view()
{
  return m_abstraction_view->get_view();
}

template<class AppArmorParser>
std::shared_ptr<Gtk::TreeView> ProfileModifyImpl<AppArmorParser>::get_file_rule_view()
{
  return m_file_rule_view->get_view();
}

template<class AppArmorParser>
void ProfileModifyImpl<AppArmorParser>::connect_apply_buttons(const void_func &cancel_button_handler, const void_func &apply_button_handler)
{
  m_cancel_button->signal_clicked().connect(cancel_button_handler);
  m_apply_button->signal_clicked().connect(apply_button_handler);
}

template<class AppArmorParser>
void ProfileModifyImpl<AppArmorParser>::connect_handle_profile_changed(const void_func &parser_handler)
{
  handle_apparmor_parser_changed = parser_handler;
}

template<class AppArmorParser>
void ProfileModifyImpl<AppArmorParser>::connect_handle_remove_rule(const void_func &ab_fun,
                                                                   const void_func &fr_fun)
{
  ab_delete_button->signal_clicked().connect(ab_fun);
  frule_delete_button->signal_clicked().connect(fr_fun);
}

template<class AppArmorParser>
void ProfileModifyImpl<AppArmorParser>::handle_apply_visible()
{
  m_button_reveal->set_reveal_child(parser->hasChanges());
}

template<class AppArmorParser>
void ProfileModifyImpl<AppArmorParser>::update_profile_text()
{
  const std::string profile_data = parser->operator std::string();
  m_profile_text->get_buffer()->set_text(profile_data);
}

template<class AppArmorParser>
void ProfileModifyImpl<AppArmorParser>::handle_raw_profile_text_change()
{
  const std::string old_profile_data = parser->operator std::string();
  const std::string new_profile_data = m_profile_text->get_buffer()->get_text();

  const bool changed = (old_profile_data != new_profile_data);
  m_raw_text_apply_reveal->set_reveal_child(changed);
}

template<class AppArmorParser>
void ProfileModifyImpl<AppArmorParser>::apply_raw_profile_text_change()
{
  const std::string new_profile_data = m_profile_text->get_buffer()->get_text();
  parser->updateFromString(new_profile_data);

  handle_apparmor_parser_changed();
  handle_raw_profile_text_change();
}

template<class AppArmorParser>
ProfileModifyImpl<AppArmorParser>::ProfileModifyImpl(std::shared_ptr<AppArmorParser> parser,
                                                     const std::shared_ptr<AppArmor::Profile> &profile)
  : builder{ Gtk::Builder::create_from_resource("/modal/profile_modify.glade") },
    m_box{ Common::get_widget<Gtk::Box>("m_box", builder) },
    m_raw_profile_title{ Common::get_widget<Gtk::Label>("m_raw_profile_title", builder) },
    m_abstraction_box{ Common::get_widget<Gtk::Box>("m_abstraction_box", builder) },
    m_file_rule_box{ Common::get_widget<Gtk::Box>("m_file_rule_box", builder) },
    m_profile_text{ Common::get_widget<Gtk::TextView>("m_profile_text", builder) },
    m_button_reveal{ Common::get_widget<Gtk::Revealer>("m_button_reveal", builder) },
    m_cancel_button{ Common::get_widget<Gtk::Button>("m_cancel_button", builder) },
    m_apply_button{ Common::get_widget<Gtk::Button>("m_apply_button", builder) },
    m_raw_text_apply_reveal{ Common::get_widget<Gtk::Revealer>("m_raw_text_apply_reveal", builder) },
    m_raw_text_cancel_button{ Common::get_widget<Gtk::Button>("m_raw_text_cancel_button", builder) },
    m_raw_text_apply_button{ Common::get_widget<Gtk::Button>("m_raw_text_apply_button", builder) },
    ab_add_button{ Common::get_widget<Gtk::Button>("ab_add_button", builder) },
    ab_delete_button{ Common::get_widget<Gtk::Button>("ab_delete_button", builder) },
    frule_add_button{ Common::get_widget<Gtk::Button>("frule_add_button", builder) },
    frule_delete_button{ Common::get_widget<Gtk::Button>("frule_delete_button", builder) },
    m_abstraction_view{ std::make_shared<ScrolledView>(profile->name()) },
    m_file_rule_view{ std::make_shared<ScrolledView>(profile->name()) },
    parser{ parser }
{
  // Set the labels that show the Profile's name
  const auto &profile_name = profile->name();
  m_raw_profile_title->set_label(profile_name);

  // Add the ScrolledView widgets
  m_abstraction_box->add(*m_abstraction_view);
  m_file_rule_box->add(*m_file_rule_view);

  // Fill the textarea for the "Profile Text" section
  update_profile_text();
  handle_raw_profile_text_change();

  // Connect buttons which change m_profile_text
  auto cancel_raw_text_change = sigc::mem_fun(*this, &ProfileModifyImpl<AppArmorParser>::update_profile_text);
  auto apply_raw_text_change  = sigc::mem_fun(*this, &ProfileModifyImpl<AppArmorParser>::apply_raw_profile_text_change);
  m_raw_text_cancel_button->signal_clicked().connect(cancel_raw_text_change);
  m_raw_text_apply_button->signal_clicked().connect(apply_raw_text_change);

  // Handle the changes in m_profile_text
  auto handle_text_change = sigc::mem_fun(*this, &ProfileModifyImpl<AppArmorParser>::handle_raw_profile_text_change);
  m_profile_text->get_buffer()->signal_changed().connect(handle_text_change);

  this->add(*m_box);
  this->show_all();
  handle_apply_visible();
}

template class ProfileModifyImpl<AppArmor::Parser>;