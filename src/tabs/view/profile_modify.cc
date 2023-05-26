#include "common.h"
#include "../../threads/command_caller.h"
#include "profile_modify.h"

#include <exception>
#include <libappanvil/apparmor_parser.hh>
#include <gtkmm/button.h>
#include <gtkmm/enums.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_set>

// Size of right-margin (pixels) for labels created by following method 
constexpr int LABEL_RIGHT_MARGIN = 8;

template<class AppArmorParser>
Gtk::Label* ProfileModifyImpl<AppArmorParser>::create_label(const std::string &text)
{
  auto *label = Gtk::make_managed<Gtk::Label>(text);
  label->set_halign(Gtk::ALIGN_START);
  label->set_margin_right(LABEL_RIGHT_MARGIN);
  label->set_ellipsize(Pango::EllipsizeMode::ELLIPSIZE_END);
  label->set_selectable();
  return label;
}

template<class AppArmorParser>
Gtk::Button* ProfileModifyImpl<AppArmorParser>::create_image_button(const std::string &image_name)
{
  auto *image = Gtk::make_managed<Gtk::Button>();
  image->set_image_from_icon_name(image_name);
  return image;
}

template<class AppArmorParser>
template<AppArmor::RuleDerived RuleType>
Gtk::Button* ProfileModifyImpl<AppArmorParser>::create_edit_button(RuleType &rule)
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
Gtk::Button* ProfileModifyImpl<AppArmorParser>::create_delete_button(RuleType &rule, const std::string &name)
{
  auto *image = create_image_button("edit-delete-symbolic");

  // Create lambda to be called when button is clicked
  auto on_clicked = [this, &rule](){
    handle_delete<RuleType>(rule);
  };
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
void ProfileModifyImpl<AppArmorParser>::intialize_abstractions()
{
  // m_abstraction_grid->clear();
  auto abstractions = profile->getAbstractions();

  int row = 0;
  for(auto &abstraction : abstractions) {
    // Find the last slash, and use that information to extract the filename from the abstraction
    std::string abstraction_str = abstraction.getPath();
    auto pos = abstraction_str.find_last_of('/');
    if(pos == std::string::npos) {
      pos = 0;
    }
    else {
      pos++;
    }

    std::string trimmed = abstraction_str.substr(pos);

    // Create the widgets to display in this row
    auto *label       = create_label(trimmed);
    auto *image_edit  = create_edit_button(abstraction);
    auto *image_trash = create_delete_button(abstraction, abstraction_str);

    // Add the widgets to the grid
    m_abstraction_grid->insert_row(row);
    m_abstraction_grid->attach(*label, 0, row);
    m_abstraction_grid->attach(*image_edit, 1, row);
    m_abstraction_grid->attach(*image_trash, 2, row);

    row++;
  }
}

template<class AppArmorParser>
void ProfileModifyImpl<AppArmorParser>::intialize_file_rules()
{
  auto rules = profile->getFileRules();

  int row = 0;
  for(AppArmor::Tree::FileRule &rule : rules) {
    const std::string filename = rule.getFilename();
    const std::string filemode = rule.getFilemode();

    auto *file_label  = create_label(filename);
    auto *mode_label  = create_label(filemode);
    auto *image_edit  = create_edit_button(rule);
    auto *image_trash = create_delete_button(rule, filename);

    mode_label->set_halign(Gtk::ALIGN_END);

    m_file_rule_grid->insert_row(row);
    m_file_rule_grid->attach(*file_label, 0, row);
    m_file_rule_grid->attach(*mode_label, 1, row);
    m_file_rule_grid->attach(*image_edit, 2, row);
    m_file_rule_grid->attach(*image_trash, 3, row);

    row++;
  }
}

template<class AppArmorParser>
ProfileModifyImpl<AppArmorParser>::ProfileModifyImpl(std::shared_ptr<AppArmorParser> parser, 
                                                                      std::shared_ptr<AppArmor::Profile> profile)
  : builder{ Gtk::Builder::create_from_resource("/resources/profile_modify.glade") },
    m_box{ Common::get_widget<Gtk::Box>("m_box", builder) },
    m_title_1{ Common::get_widget<Gtk::Label>("m_title_1", builder) },
    m_title_2{ Common::get_widget<Gtk::Label>("m_title_2", builder) },
    m_abstraction_grid{ Common::get_widget<Gtk::Grid>("m_abstraction_grid", builder) },
    m_file_rule_grid{ Common::get_widget<Gtk::Grid>("m_file_rule_grid", builder) },
    parser{ parser },
    profile{ profile }
{
  const auto &profile_name = profile->name();
  m_title_1->set_label(profile_name);
  m_title_2->set_label(profile_name);

  // Left justify the children of this box
  m_abstraction_grid->set_halign(Gtk::ALIGN_START);
  m_abstraction_grid->set_valign(Gtk::ALIGN_START);

  // Initialize the lists of abstractions and file_rules using the parsed profile
  intialize_abstractions();
  intialize_file_rules();

  this->add(*m_box);
  this->show_all();
}

template class ProfileModifyImpl<AppArmor::Parser>;