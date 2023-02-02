#ifndef TABS_PROFILES_H
#define TABS_PROFILES_H

#include "profile_loader.h"
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

  // Sets the function to be used when changing the status of a profile, this is used in main_window.cc
  void set_status_change_signal_handler(sigc::slot<void(std::string, std::string, std::string)> change_fun);

  /**
   * @brief Change the text in the label next to the Apply button/spinner.
   */
  void set_apply_label_text(const std::string &str);

  void set_profile_info(const std::string &num_logs, const std::string &num_procs);

  void show_profile_info();
  void hide_profile_info();

protected:
  // Signal handler to handle when the user wants to change the status of a profile
  // This calls the default_change_fun with the correct values for the profile, old_status, and new_status
  void change_status();

  void handle_load_profile_toggle();
  void handle_change_state_toggle();
  void handle_modify_profile_toggle();

private:
  sigc::slot<void(std::string, std::string, std::string)> profile_status_change_fun;

  // Widgets
  Glib::RefPtr<Gtk::Builder> builder;

  std::unique_ptr<Gtk::ToggleButton> p_change_state_toggle;
  std::unique_ptr<Gtk::ToggleButton> p_load_profile_toggle;
  std::unique_ptr<Gtk::ToggleButton> p_modify_profile_toggle;

  std::unique_ptr<Gtk::Stack> p_stack;
  std::unique_ptr<Gtk::Box> p_state_selection_box;
  std::unique_ptr<Gtk::ComboBoxText> p_status_selection;
  std::unique_ptr<Gtk::Button> p_apply_button;
  std::unique_ptr<Gtk::Label> p_apply_info_text;

  std::unique_ptr<Gtk::Box> p_profile_info;
  std::unique_ptr<Gtk::Label> p_num_log_label;
  std::unique_ptr<Gtk::Label> p_num_proc_label;

  // Additional pages, which are added to the stack
  std::unique_ptr<ProfileLoader> loader;
  std::map<std::string, std::shared_ptr<ProfileModify>> modifiers;

  // Map of all known profiles
  std::map<std::string, AppArmor::Profile> profile_map;

#ifdef TESTS_ENABLED
  FRIEND_TEST(ProfilesTest, CHECK_APPLY_LABEL_TEXT);
  FRIEND_TEST(ProfilesTest, CHANGE_STATUS_WIDGETS_INVISIBLE_WHEN_NO_ROWS_SELECTED);
  FRIEND_TEST(ProfilesTest, CHANGE_STATUS_WIDGETS_INVISIBLE_WHEN_CHANGE_TOGGLE_NOT_PRESSED);
  FRIEND_TEST(ProfilesTest, CHANGE_STATUS_WIDGETS_VISIBLE_WHEN_CHANGE_TOGGLE_PRESSED);
  FRIEND_TEST(ProfilesTest, CHANGE_STATUS_NO_ROWS_SELECTED);
  FRIEND_TEST(ProfilesTest, CHANGE_STATUS_ROW_SELECTED);
  FRIEND_TEST(ProfilesTest, SHOW_PROFILE_INFO_MAKES_CHANGE_TOGGLE_VISIBLE);
  FRIEND_TEST(ProfilesTest, HIDE_PROFILE_INFO_MAKES_CHANGE_TOGGLE_INVISIBLE);
  FRIEND_TEST(ProfilesTest, NO_SIMULTANEOUS_TOGGLE_PRESS);
  FRIEND_TEST(ProfilesTest, NO_SIMULTANEOUS_TOGGLE_PRESS_TWO);
  FRIEND_TEST(ProfilesTest, SET_PROFILE_INFO);
#endif
};

#endif // TABS_PROFILES_H
