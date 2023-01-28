#include "common.h"
#include "../../threads/command_caller.h"
#include "profile_modify.h"

#include <apparmor_file_rule.hh>
#include <apparmor_parser.hh>
#include <apparmor_profile.hh>
#include <gtkmm/button.h>
#include <gtkmm/enums.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>
#include <memory>
#include <string>
#include <unordered_set>

// Size of right-margin (pixels) for labels created by following method 
constexpr int LABEL_RIGHT_MARGIN = 8;

Gtk::Label* ProfileModify::create_label(const std::string &text)
{
  auto *label = Gtk::make_managed<Gtk::Label>(text);
  label->set_halign(Gtk::ALIGN_START);
  label->set_margin_right(LABEL_RIGHT_MARGIN);
  label->set_ellipsize(Pango::EllipsizeMode::ELLIPSIZE_END);
  return label;
}

Gtk::Button* ProfileModify::create_image_button(const std::string &image_name)
{
  auto *image = Gtk::make_managed<Gtk::Button>();
  image->set_image_from_icon_name(image_name);
  return image;
}

void ProfileModify::intialize_abstractions(const AppArmor::Profile &profile)
{
  auto abstractions = profile.getAbstractions();

  int row = 0;
  for(const std::string &abstraction : abstractions) {
    auto *label       = create_label(abstraction);
    auto *image_edit  = create_image_button("edit-symbolic");
    auto *image_trash = create_image_button("edit-delete-symbolic");   

    m_abstraction_grid->insert_row(row);
    m_abstraction_grid->attach(*label, 0, row);
    m_abstraction_grid->attach(*image_edit, 1, row);
    m_abstraction_grid->attach(*image_trash, 2, row);

    row++;
  }
}

void ProfileModify::intialize_file_rules(const AppArmor::Profile &profile)
{
  auto rules = profile.getFileRules();

  int row = 0;
  for(const auto &rule : rules) {
    const std::string filename = rule.getFilename();
    const std::string filemode = rule.getFilemode();

    auto *file_label  = create_label(filename);
    auto *mode_label  = create_label(filemode);
    auto *image_edit  = create_image_button("edit-symbolic");
    auto *image_trash = create_image_button("edit-delete-symbolic");

    mode_label->set_halign(Gtk::ALIGN_END);

    m_file_rule_grid->insert_row(row);
    m_file_rule_grid->attach(*file_label, 0, row);
    m_file_rule_grid->attach(*mode_label, 1, row);
    m_file_rule_grid->attach(*image_edit, 2, row);
    m_file_rule_grid->attach(*image_trash, 3, row);

    row++;
  }
}

ProfileModify::ProfileModify(const AppArmor::Profile &profile)
  : builder{ Gtk::Builder::create_from_resource("/resources/profile_modify.glade") },
    m_box{ Common::get_widget<Gtk::Box>("m_box", builder) },
    m_title{ Common::get_widget<Gtk::Label>("m_title", builder) },
    m_abstraction_grid{ Common::get_widget<Gtk::Grid>("m_abstraction_grid", builder) },
    m_file_rule_grid{ Common::get_widget<Gtk::Grid>("m_file_rule_grid", builder) }
{
  const auto &profile_name = profile.name();
  m_title->set_label(profile_name);

  // Left justify the children of this box
  m_abstraction_grid->set_halign(Gtk::ALIGN_START);
  m_abstraction_grid->set_valign(Gtk::ALIGN_START);

  // Initialize the lists of abstractions and file_rules using the parsed profile
  intialize_abstractions(profile);
  intialize_file_rules(profile);

  this->add(*m_box);
  this->show_all();
}
