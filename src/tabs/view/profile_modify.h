#ifndef TABS_PROFILE_MODIFY_H
#define TABS_PROFILE_MODIFY_H

#include "scrolled_view.h"

#include <gtkmm/box.h>
#include <gtkmm/builder.h>
#include <gtkmm/button.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>
#include <gtkmm/revealer.h>
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
  ProfileModifyImpl(std::shared_ptr<AppArmorParser> parser, const std::shared_ptr<AppArmor::Profile> &profile);

  std::shared_ptr<Gtk::TreeView> get_abstraction_view();
  std::shared_ptr<Gtk::TreeView> get_file_rule_view();

  typedef sigc::slot<void> void_func;
  void connect_apply_buttons(const void_func &cancel_button_handler, const void_func &apply_button_handler);

  // Connect function which will be called when the AppArmorParser is updated internally
  // This happens if a user manually edits the profile in the "Profile Text" section
  void connect_handle_profile_changed(const void_func &parser_handler);

  // Function to connect "delete rule" buttons
  void connect_handle_remove_rule(const void_func &ab_fun, const void_func &fr_fun);

  // Function to connect "add rule" buttons
  void connect_handle_add_rule(const sigc::slot<void, AppArmor::AbstractionRule> &ab_fun,
                               const sigc::slot<void, AppArmor::FileRule> &fr_fun);

  // Decides whether the apply and cancel buttons should be visible
  void handle_apply_visible();

  // Overwrites the data in the Gtk::TreeView, which shows the file this profile is in.
  // Uses the data from AppArmor::Parser
  void update_profile_text();

protected:
  // Called when the buffer in 'm_profile_text' is changed
  void handle_raw_profile_text_change();

  // Called when 'm_raw_text_apply_button' is pressed
  // Parses the text buffer in 'm_profile_text', attempting to apply it to the profile
  void apply_raw_profile_text_change();

  // Called when 'ab_add_button' is clicked
  void handle_add_abstraction();

  void handle_add_file_rule();

private:
  // GUI Builder to parse UI from xml file
  Glib::RefPtr<Gtk::Builder> builder;

  // VBox which holds all the widgets
  std::unique_ptr<Gtk::Box> m_box;

  // Widgets
  std::unique_ptr<Gtk::Label> m_raw_profile_title;
  std::shared_ptr<Gtk::Box> m_abstraction_box;
  std::shared_ptr<Gtk::Box> m_file_rule_box;
  std::shared_ptr<Gtk::TextView> m_profile_text;
  std::shared_ptr<Gtk::Revealer> m_button_reveal;
  std::shared_ptr<Gtk::Button> m_cancel_button;
  std::shared_ptr<Gtk::Button> m_apply_button;
  std::shared_ptr<Gtk::Revealer> m_raw_text_apply_reveal;
  std::shared_ptr<Gtk::Button> m_raw_text_cancel_button;
  std::shared_ptr<Gtk::Button> m_raw_text_apply_button;

  std::shared_ptr<Gtk::Button> ab_add_button;
  std::shared_ptr<Gtk::Button> ab_delete_button;
  std::shared_ptr<Gtk::Button> frule_add_button;
  std::shared_ptr<Gtk::Button> frule_delete_button;

  std::shared_ptr<ScrolledView> m_abstraction_view;
  std::shared_ptr<ScrolledView> m_file_rule_view;

  // Function that will be called if parser is updated internally
  void_func handle_apparmor_parser_changed = []() {};

  sigc::slot<void, AppArmor::AbstractionRule &> add_abstraction = [](AppArmor::AbstractionRule &) {};
  sigc::slot<void, AppArmor::FileRule &> add_file_rule          = [](AppArmor::FileRule &) {};

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
