#ifndef TABS_PROFILES_H
#define TABS_PROFILES_H

#include "../controller/profile_loader_controller.h"
#include "../controller/profile_modify_controller.h"
#include "profile_modify.h"
#include "status.h"

#include <gtkmm/box.h>
#include <gtkmm/stack.h>
#include <gtkmm/togglebutton.h>
#include <memory>
#include <string>
#include <vector>

#ifdef TESTS_ENABLED
#include <gtest/gtest.h>
#endif

class Profiles : public Status
{
public:
  explicit Profiles();

  void set_profile_info(const std::string &num_logs, const std::string &num_procs);

  // Given a profile name, return the filepath of the profile (if it was found)
  std::string find_path(const std::string &profile_name);

  void show_profile_info();
  void hide_profile_info();

protected:
  void handle_load_profile_toggle();
  void handle_change_state_toggle();
  void handle_modify_profile_toggle();

private:
  sigc::slot<void(std::string, std::string, std::string)> profile_status_change_fun;

  // Widgets
  Glib::RefPtr<Gtk::Builder> builder;

  std::unique_ptr<Gtk::ToggleButton> p_load_profile_toggle;
  std::unique_ptr<Gtk::ToggleButton> p_modify_profile_toggle;

  std::unique_ptr<Gtk::Stack> p_stack;

  std::unique_ptr<Gtk::Box> p_profile_info;
  std::unique_ptr<Gtk::Label> p_num_log_label;
  std::unique_ptr<Gtk::Label> p_num_proc_label;

  // Additional pages, which are added to the stack
  std::unique_ptr<ProfileLoaderController> loader_controller;
  std::map<std::string, std::shared_ptr<ProfileModifyController>> modifiers;

  // Map of all known profiles
  std::map<std::string, std::pair<AppArmor::Parser, AppArmor::Profile>> profile_map;

#ifdef TESTS_ENABLED
  FRIEND_TEST(ProfilesTest, CHECK_APPLY_LABEL_TEXT);
  FRIEND_TEST(ProfilesTest, CHANGE_STATUS_WIDGETS_INVISIBLE_WHEN_NO_ROWS_SELECTED);
  FRIEND_TEST(ProfilesTest, CHANGE_STATUS_WIDGETS_INVISIBLE_WHEN_CHANGE_TOGGLE_NOT_PRESSED);
  FRIEND_TEST(ProfilesTest, CHANGE_STATUS_WIDGETS_VISIBLE_WHEN_CHANGE_TOGGLE_PRESSED);
  FRIEND_TEST(ProfilesTest, CHANGE_STATUS_NO_ROWS_SELECTED);
  FRIEND_TEST(ProfilesTest, CHANGE_STATUS_ROW_SELECTED);
  FRIEND_TEST(ProfilesTest, SHOW_PROFILE_INFO_MAKES_WITHOUT_PROFILE_FOUND);
  FRIEND_TEST(ProfilesTest, HIDE_PROFILE_INFO_MAKES_CHANGE_TOGGLE_INVISIBLE);
  FRIEND_TEST(ProfilesTest, NO_SIMULTANEOUS_TOGGLE_PRESS);
  FRIEND_TEST(ProfilesTest, NO_SIMULTANEOUS_TOGGLE_PRESS_TWO);
  FRIEND_TEST(ProfilesTest, SET_PROFILE_INFO);
#endif
};

#endif // TABS_PROFILES_H
