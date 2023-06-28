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

// Size of right-margin (pixels) for labels created by following method
constexpr int LABEL_RIGHT_MARGIN = 8;

template<class AppArmorParser>
Gtk::Label *ProfileModifyImpl<AppArmorParser>::create_label(const std::string &text)
{
  auto *label = Gtk::make_managed<Gtk::Label>(text);
  label->set_halign(Gtk::ALIGN_START);
  label->set_margin_right(LABEL_RIGHT_MARGIN);
  label->set_ellipsize(Pango::EllipsizeMode::ELLIPSIZE_END);
  label->set_selectable();
  return label;
}

template<class AppArmorParser>
Gtk::Button *ProfileModifyImpl<AppArmorParser>::create_image_button(const std::string &image_name)
{
  auto *image = Gtk::make_managed<Gtk::Button>();
  image->set_image_from_icon_name(image_name);
  return image;
}

template<class AppArmorParser>
template<AppArmor::RuleDerived RuleType>
Gtk::Button *ProfileModifyImpl<AppArmorParser>::create_edit_button(RuleType &rule)
{
  auto *image = create_image_button("edit-symbolic");

  // // Create lambda to be called when button is clicked
  // auto on_clicked = [name](){
  //   std::cout << "Edit Clicked (this function is not implemented): " << name << std::endl;
  // };
  // image->signal_clicked().connect(on_clicked);

  return image;
}

template<class AppArmorParser>
template<AppArmor::RuleDerived RuleType>
Gtk::Button *ProfileModifyImpl<AppArmorParser>::create_delete_button(RuleType &rule, const std::string &name)
{
  auto *image = create_image_button("edit-delete-symbolic");

  // Create lambda to be called when button is clicked
  auto on_clicked = [this, &rule]() { handle_delete<RuleType>(rule); };
  image->signal_clicked().connect(on_clicked);

  return image;
}

template<class AppArmorParser>
template<AppArmor::RuleDerived RuleType>
void ProfileModifyImpl<AppArmorParser>::handle_delete(RuleType &rule)
{
  try {
    parser->removeRule(*profile, rule);
  } catch (std::domain_error ex) {
    std::cerr << ex.what() << std::endl;
  }
}

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
ProfileModifyImpl<AppArmorParser>::ProfileModifyImpl(std::shared_ptr<AppArmorParser> parser, std::shared_ptr<AppArmor::Profile> profile)
  : builder{ Gtk::Builder::create_from_resource("/resources/profile_modify.glade") },
    m_box{ Common::get_widget<Gtk::Box>("m_box", builder) },
    m_title_1{ Common::get_widget<Gtk::Label>("m_title_1", builder) },
    m_title_2{ Common::get_widget<Gtk::Label>("m_title_2", builder) },
    m_abstraction_view{ Common::get_widget<Gtk::TreeView>("m_abstraction_view", builder) },
    m_file_rule_view{ Common::get_widget<Gtk::TreeView>("m_file_rule_view", builder) },
    parser{ parser },
    profile{ profile }
{
  const auto &profile_name = profile->name();
  m_title_1->set_label(profile_name);
  m_title_2->set_label(profile_name);

  this->add(*m_box);
  this->show_all();
}

template class ProfileModifyImpl<AppArmor::Parser>;