#ifndef TABS_PROFILE_MODIFY_H
#define TABS_PROFILE_MODIFY_H

#include <gtkmm/box.h>
#include <gtkmm/builder.h>
#include <gtkmm/button.h>
#include <gtkmm/grid.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/textview.h>
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

  // Decides whether the apply and cancel buttons should be visible
  void handle_apply_visible();

  // Overwrites the data in the Gtk::TreeView, which shows the file this profile is in.
  // Uses the data from AppArmor::Parser
  void update_profile_text();

private:
  // GUI Builder to parse UI from xml file
  Glib::RefPtr<Gtk::Builder> builder;

  // VBox which holds all the widgets
  std::unique_ptr<Gtk::Box> m_box;

  // Widgets
  std::unique_ptr<Gtk::Label> m_title_1;
  std::unique_ptr<Gtk::Label> m_title_2;
  std::unique_ptr<Gtk::Label> m_title_3;
  std::shared_ptr<Gtk::TreeView> m_abstraction_view;
  std::shared_ptr<Gtk::TreeView> m_file_rule_view;
  std::shared_ptr<Gtk::TextView> m_profile_text;
  std::shared_ptr<Gtk::Box> m_button_box;
  std::shared_ptr<Gtk::Button> m_cancel_button;
  std::shared_ptr<Gtk::Button> m_apply_button;

  // Fields used for reading and modifying the profile
  std::shared_ptr<AppArmorParser> parser;

#ifdef TESTS_ENABLED
  FRIEND_TEST(ProfileModifyTest, TEST_CONSTRUCTOR);
  FRIEND_TEST(ProfileModifyTest, TEST_DELETE_BUTTON);
  FRIEND_TEST(ProfileModifyTest, TEST_DELETE_BUTTON_WITH_EXCEPTION);
#endif
};

typedef ProfileModifyImpl<AppArmor::Parser> ProfileModify;

#endif
