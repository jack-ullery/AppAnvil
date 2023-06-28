#ifndef TABS_PROFILE_MODIFY_H
#define TABS_PROFILE_MODIFY_H

#include <gtkmm/box.h>
#include <gtkmm/builder.h>
#include <gtkmm/button.h>
#include <gtkmm/grid.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/treeview.h>
#include <libappanvil/apparmor_parser.hh>
#include <libappanvil/tree/AbstractionRule.hh>
#include <libappanvil/tree/FileRule.hh>
#include <list>
#include <memory>
#include <string>
#include <tuple>

#ifdef TESTS_ENABLED
#include <gtest/gtest.h>
#endif

template<class AppArmorParser>
class ProfileModifyImpl : public Gtk::ScrolledWindow
{
public:
  ProfileModifyImpl(std::shared_ptr<AppArmorParser> parser, std::shared_ptr<AppArmor::Profile> profile);

  std::shared_ptr<Gtk::TreeView> get_abstraction_view();
  std::shared_ptr<Gtk::TreeView> get_file_rule_view();

protected:
  // Functions to create certain reused Gtk Widgets
  // Each function returns a pointer that will be freed by Gtk
  static Gtk::Label *create_label(const std::string &text);
  static Gtk::Button *create_image_button(const std::string &image_name);

  template<AppArmor::RuleDerived RuleType>
  Gtk::Button *create_edit_button(RuleType &rule);

  template<AppArmor::RuleDerived RuleType>
  Gtk::Button *create_delete_button(RuleType &rule, const std::string &name);

  // Helper functions that will be called when Profile rules should be changed
  template<AppArmor::RuleDerived RuleType>
  void handle_delete(RuleType &rule);

private:
  // GUI Builder to parse UI from xml file
  Glib::RefPtr<Gtk::Builder> builder;

  // VBox which holds all the widgets
  std::unique_ptr<Gtk::Box> m_box;

  // Widgets
  std::unique_ptr<Gtk::Label> m_title_1;
  std::unique_ptr<Gtk::Label> m_title_2;
  std::shared_ptr<Gtk::TreeView> m_abstraction_view;
  std::shared_ptr<Gtk::TreeView> m_file_rule_view;

  // Typedef a tuple of widgets
  typedef std::tuple<std::shared_ptr<Gtk::Widget>, std::shared_ptr<Gtk::Widget>, std::shared_ptr<Gtk::Widget>> widget_tuple;

  // Fields used for reading and modifying the profile
  std::shared_ptr<AppArmorParser> parser;
  std::shared_ptr<AppArmor::Profile> profile;

  // Container of added abstractions and file rules
  std::map<std::string, widget_tuple> abstraction_map;
  std::map<std::shared_ptr<AppArmor::Tree::FileRule>, std::shared_ptr<Gtk::Widget>> file_rule_map;

#ifdef TESTS_ENABLED
  FRIEND_TEST(ProfileModifyTest, TEST_CONSTRUCTOR);
  FRIEND_TEST(ProfileModifyTest, TEST_DELETE_BUTTON);
  FRIEND_TEST(ProfileModifyTest, TEST_DELETE_BUTTON_WITH_EXCEPTION);
#endif
};

typedef ProfileModifyImpl<AppArmor::Parser> ProfileModify;

#endif
