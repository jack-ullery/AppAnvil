#ifndef TABS_PROFILES_H
#define TABS_PROFILES_H

#include "profile_loader.h"
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

  // Sets the function to be called when the `p_load_profile` button is pressed
  void set_load_profile_signal_handler(sigc::slot<void> change_fun);

  /**
   * @brief Change the text in the label next to the Apply button/spinner.
   */
  void set_apply_label_text(const std::string &str);

  void set_profile_info(const std::string &num_logs, const std::string &num_perms, const std::string &num_procs);

  void show_profile_info();
  void hide_profile_info();

protected:
  // Signal handler to handle when the user wants to change the status of a profile
  // This calls the default_change_fun with the correct values for the profile, old_status, and new_status
  void change_status();

  void handle_load_profile_toggle();
  void handle_change_state_toggle();

private:
  sigc::slot<void(std::string, std::string, std::string)> profile_status_change_fun;

  // Widgets
  Glib::RefPtr<Gtk::Builder> builder;

  std::shared_ptr<Gtk::ToggleButton> p_change_state_toggle;
  std::shared_ptr<Gtk::ToggleButton> p_load_profile_toggle;

  std::shared_ptr<Gtk::Stack> p_stack;
  std::shared_ptr<Gtk::Box> p_state_selection_box;
  std::shared_ptr<Gtk::ComboBoxText> p_status_selection;
  std::shared_ptr<Gtk::Button> p_apply_button;
  std::shared_ptr<Gtk::Label> p_apply_info_text;

  std::shared_ptr<Gtk::Box> p_profile_info;
  std::shared_ptr<Gtk::Label> p_num_log_label;
  std::shared_ptr<Gtk::Label> p_num_proc_label;
  std::shared_ptr<Gtk::Label> p_num_perm_label;

  // Profile Loader page, which is added to the stack
  std::unique_ptr<ProfileLoader> loader;

  // Misc
  template<typename T_Widget>
  static std::shared_ptr<T_Widget> get_widget_shared(Glib::ustring name, const Glib::RefPtr<Gtk::Builder> &builder);

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
  #endif
};

#endif // TABS_PROFILES_H
