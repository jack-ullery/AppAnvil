#ifndef TABS_PROFILE_MODIFY_H
#define TABS_PROFILE_MODIFY_H

#include <libappanvil/apparmor_parser.hh>
#include <libappanvil/tree/AbstractionRule.hh>
#include <libappanvil/tree/FileRule.hh>
#include <gtkmm/box.h>
#include <gtkmm/builder.h>
#include <gtkmm/button.h>
#include <gtkmm/grid.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>
#include <gtkmm/scrolledwindow.h>
#include <list>
#include <memory>
#include <string>
#include <tuple>

class ProfileModify : public Gtk::ScrolledWindow
{
public:
  explicit ProfileModify(AppArmor::Parser &parser, AppArmor::Profile &profile);

protected:
  // Non-static helper functions
  void intialize_abstractions();
  void intialize_file_rules();

private:
  // GUI Builder to parse UI from xml file
  Glib::RefPtr<Gtk::Builder> builder;

  // Functions to create certain reused Gtk Widgets
  // Each function returns a pointer that will be freed by Gtk
  static Gtk::Label* create_label(const std::string &text);
  static Gtk::Button* create_image_button(const std::string &image_name);

  template<AppArmor::RuleDerived RuleType>
  Gtk::Button* create_edit_button(RuleType &rule);

  template<AppArmor::RuleDerived RuleType>
  Gtk::Button* create_delete_button(RuleType &rule, const std::string &name);

  // Helper functions that will be called when Profile rules should be changed
  template<AppArmor::RuleDerived RuleType>
  void handle_delete(RuleType &rule);

  // VBox which holds all the widgets
  std::unique_ptr<Gtk::Box> m_box;

  // Widgets
  std::unique_ptr<Gtk::Label> m_title_1;
  std::unique_ptr<Gtk::Label> m_title_2;
  std::unique_ptr<Gtk::Grid>  m_abstraction_grid;
  std::unique_ptr<Gtk::Grid>  m_file_rule_grid;

  // Typedef a tuple of widgets
  typedef std::tuple<std::shared_ptr<Gtk::Widget>, std::shared_ptr<Gtk::Widget>, std::shared_ptr<Gtk::Widget>> widget_tuple;

  // Fields used for reading and modifying the profile
  AppArmor::Parser parser;
  AppArmor::Profile profile;

  // Container of added abstractions and file rules
  std::map<std::string, widget_tuple> abstraction_map;
  std::map<std::shared_ptr<AppArmor::Tree::FileRule>, std::shared_ptr<Gtk::Widget>> file_rule_map;
};

#endif
