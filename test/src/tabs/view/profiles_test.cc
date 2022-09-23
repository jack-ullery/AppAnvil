#include "profiles_test.h"

#include <chrono>
#include <gtest/gtest.h>
#include <gtkmm/comboboxtext.h>
#include <memory>
#include <thread>

using ::testing::_;

Glib::RefPtr<Gtk::TreeStore>
ProfilesTest::initialize_store()
{
  auto first_column  = Gtk::TreeModelColumn<std::string>();
  auto second_column = Gtk::TreeModelColumn<std::string>();

  record.add(first_column);
  record.add(second_column);

  auto view = pc.get_view();
  EXPECT_FALSE(view == nullptr) << "`pc.get_view()` should not return a nullptr";

  view->append_column("Profile", first_column);
  view->append_column("Status", second_column);

  auto store = Gtk::TreeStore::create(record);
  view->set_model(store);

  return store;
}

void
ProfilesTest::create_and_select_row(Glib::RefPtr<Gtk::TreeStore> store, std::string profile, std::string status)
{
  // Create a row and put some data into the first column
  auto iter = store->append();
  iter->set_value(0, profile);
  iter->set_value(1, status);

  // Get the path to the newly created row
  auto path = store->get_path(iter);

  // Get the view
  auto view = pc.get_view();
  EXPECT_FALSE(view == nullptr) << "`pc.get_view()` should not return a nullptr";

  // Select the newly created row
  view->get_selection()->select(path);
}

TEST_F(ProfilesTest, CHECK_APPLY_LABEL_TEXT)
{
  std::string label_text = "arbitrary text for apply label";

  bool label_exists = (pc.p_apply_info_text->get_text().compare(label_text) == 0);
  EXPECT_FALSE(label_exists);

  pc.set_apply_label_text(label_text);

  label_exists = (pc.p_apply_info_text->get_text().compare(label_text) == 0);
  EXPECT_TRUE(label_exists);
}

TEST_F(ProfilesTest, CHANGE_STATUS_WIDGETS_INVISIBLE_WHEN_NO_ROWS_SELECTED)
{
  // Since there were no rows selected, the following widgets should not be visible
  ASSERT_TRUE(pc.p_load_profile_toggle->is_visible()) << "This toggle should always be visible";

  ASSERT_FALSE(pc.p_change_state_toggle->get_sensitive())
    << "Since there were no rows selected, the change_state toggle button should not be enabled";

  ASSERT_FALSE(pc.p_state_selection_box->is_visible())
    << "Since there were no rows selected, the Widget 'p_state_selection_box' should not be visible";

  ASSERT_FALSE(pc.p_apply_button->is_visible()) << "Since there were no rows selected, the apply button should not be visible";

  ASSERT_FALSE(pc.p_apply_info_text->is_visible())
    << "Since there were no rows selected, the Widget 'p_apply_info_text' should not be visible";
}

TEST_F(ProfilesTest, CHANGE_STATUS_WIDGETS_INVISIBLE_WHEN_CHANGE_TOGGLE_NOT_PRESSED)
{
  auto profile_name = "Test_Profile_Name";
  auto status       = "fake_status";

  auto store = initialize_store();
  create_and_select_row(store, profile_name, status);

  // This toggle should not be pressed by default
  ASSERT_FALSE(pc.p_change_state_toggle->get_active()) << "This toggle should not be pressed by default, even when a row is selected";

  // Since the change status toggle is not pressed, these widgets should not be visible
  ASSERT_FALSE(pc.p_state_selection_box->is_visible())
    << "Since the change status toggle is not pressed, the Widget 'p_state_selection_box' should not be visible";

  ASSERT_FALSE(pc.p_apply_button->is_visible()) << "Since the change status toggle is not pressed, the apply button should not be visible";

  ASSERT_FALSE(pc.p_apply_info_text->is_visible())
    << "Since the change status toggle is not pressed, the Widget 'p_apply_info_text' should not be visible";
}

TEST_F(ProfilesTest, CHANGE_STATUS_WIDGETS_VISIBLE_WHEN_CHANGE_TOGGLE_PRESSED)
{
  auto profile_name = "Test_Profile_Name";
  auto status       = "fake_status";

  auto store = initialize_store();
  create_and_select_row(store, profile_name, status);

  // Press the toggle
  pc.p_change_state_toggle->set_active(true);

  // Since the change status toggle is pressed, these widgets should be visible
  ASSERT_TRUE(pc.p_state_selection_box->is_visible())
    << "Since the change status toggle is pressed, the Widget 'p_state_selection_box' should be visible";

  ASSERT_TRUE(pc.p_apply_button->is_visible()) << "Since the change status toggle is pressed, the apply button should be visible";

  ASSERT_TRUE(pc.p_apply_info_text->is_visible())
    << "Since the change status toggle is pressed, the Widget 'p_apply_info_text' should be visible";
}

TEST_F(ProfilesTest, CHANGE_STATUS_NO_ROWS_SELECTED)
{
  // Since there are no rows selected, handle_signal should not be called
  EXPECT_CALL(*this, handle_signal(_, _, _)).Times(0);

  // Connect the mocked function to the signal handler
  auto fun = sigc::mem_fun(*this, &ProfilesTest::handle_signal);
  pc.set_status_change_signal_handler(fun);

  // Call the change_status function (which should be called when the apply button is pressed)
  pc.change_status();

  // Since there were no rows selected, there should be some text on the apply label
  ASSERT_EQ(pc.p_apply_info_text->get_text(), "Please select a row.")
    << "Since there were no rows selected, we expect the above message on the apply label";
}

TEST_F(ProfilesTest, CHANGE_STATUS_ROW_SELECTED)
{
  auto profile_name = "Test_Profile_Name";
  auto old_status   = "fake_status";
  auto new_status   = "Complain";

  auto store = initialize_store();
  create_and_select_row(store, profile_name, old_status);

  // Since there is one rows selected, handle_signal should be called once, with the correct profile name
  EXPECT_CALL(*this, handle_signal(profile_name, old_status, "complain")).Times(1);

  // Connect the mocked function to the signal handler
  auto fun = sigc::mem_fun(*this, &ProfilesTest::handle_signal);
  pc.set_status_change_signal_handler(fun);

  // Set the combobox selection to new_status
  pc.p_status_selection->set_active_text(new_status);

  // Call the change_status function (which should be called when the apply button is pressed)
  pc.change_status();
}

TEST_F(ProfilesTest, SHOW_PROFILE_INFO_MAKES_CHANGE_TOGGLE_VISIBLE)
{
  ASSERT_TRUE(pc.p_load_profile_toggle->is_visible()) << "This toggle should always be visible";

  ASSERT_FALSE(pc.p_change_state_toggle->get_sensitive()) << "This toggle should not be enabled by default";

  ASSERT_FALSE(pc.p_change_state_toggle->get_active()) << "This toggle should not be active by default";

  // This should make the toggle visible
  pc.show_profile_info();

  ASSERT_TRUE(pc.p_load_profile_toggle->is_visible()) << "This toggle should always be visible";

  ASSERT_TRUE(pc.p_change_state_toggle->get_sensitive()) << "This toggle should be enabled after `show_profile_info()` is called";

  ASSERT_FALSE(pc.p_change_state_toggle->get_active()) << "This toggle should be active after `show_profile_info()` is called";
}

TEST_F(ProfilesTest, HIDE_PROFILE_INFO_MAKES_CHANGE_TOGGLE_INVISIBLE)
{
  pc.p_change_state_toggle->show();
  pc.p_change_state_toggle->set_active(true);

  // This should hide the toggle
  pc.hide_profile_info();

  ASSERT_TRUE(pc.p_load_profile_toggle->is_visible()) << "This toggle should always be visible";

  ASSERT_FALSE(pc.p_change_state_toggle->get_sensitive()) << "This toggle should not be enabled after `hide_profile_info()` is called";

  ASSERT_FALSE(pc.p_change_state_toggle->get_active()) << "This toggle should not be active after `hide_profile_info()` is called";
}

TEST_F(ProfilesTest, NO_SIMULTANEOUS_TOGGLE_PRESS)
{
  // When the tab is constructed, only the load_profile toggle should be visible
  ASSERT_TRUE(pc.p_load_profile_toggle->is_visible()) << "This toggle should always be visible";

  ASSERT_TRUE(pc.p_load_profile_toggle->is_visible()) << "This toggle should always be visible";

  ASSERT_FALSE(pc.p_change_state_toggle->get_sensitive()) << "This toggle should not be enabled by default";

  // Neither toggles should be active unless a user presses them
  ASSERT_FALSE(pc.p_load_profile_toggle->get_active()) << "This toggle should not be active by default";

  ASSERT_FALSE(pc.p_change_state_toggle->get_active()) << "This toggle should not be active by default";

  // Set the change_status toggle to be visible
  // This happens normally when a row is selected
  pc.show_profile_info();

  // Simulate activating the change_status toggle
  pc.p_change_state_toggle->set_active(true);

  // Only the change_status toggle should be pressed
  ASSERT_FALSE(pc.p_load_profile_toggle->get_active()) << "This toggle should not be active, because it has not been pressed yet";

  ASSERT_TRUE(pc.p_change_state_toggle->get_active()) << "This toggle should be active because it was just pressed";

  // Simulate activating the load_profile toggle
  pc.p_load_profile_toggle->set_active(true);

  // Only the load_profile toggle should be pressed
  // We are testing that neither of the toggles can be pressed simultaneously
  ASSERT_TRUE(pc.p_load_profile_toggle->get_active()) << "This toggle should be active because it was just pressed";

  ASSERT_FALSE(pc.p_change_state_toggle->get_active())
    << "This toggle should not be active, because it was not the most recent toggle pressed";

  // Finally untoggle the load_profile toggle
  pc.p_load_profile_toggle->set_active(false);

  // Neither toggle should be active
  ASSERT_FALSE(pc.p_load_profile_toggle->get_active()) << "This toggle should be active because it was just un-pressed";

  ASSERT_FALSE(pc.p_change_state_toggle->get_active())
    << "This toggle should not be active, because was not the most recent toggle pressed";
}

TEST_F(ProfilesTest, NO_SIMULTANEOUS_TOGGLE_PRESS_TWO)
{
  // When the tab is constructed, only the load_profile toggle should be visible
  ASSERT_TRUE(pc.p_load_profile_toggle->is_visible()) << "This toggle should be visible by default";

  ASSERT_TRUE(pc.p_load_profile_toggle->is_visible()) << "This toggle should always be visible";

  ASSERT_FALSE(pc.p_change_state_toggle->get_sensitive()) << "This toggle should not be enabled by default";

  // Neither toggles should be active unless a user presses them
  ASSERT_FALSE(pc.p_load_profile_toggle->get_active()) << "This toggle should not be active by default";

  ASSERT_FALSE(pc.p_change_state_toggle->get_active()) << "This toggle should not be active by default";

  // Set the change_status toggle to be visible
  // This happens normally when a row is selected
  pc.show_profile_info();

  // Simulate activating the load_profile toggle
  pc.p_load_profile_toggle->set_active(true);

  // Only the load_profile toggle should be pressed
  ASSERT_TRUE(pc.p_load_profile_toggle->get_active()) << "This toggle should be active because it was just pressed";

  ASSERT_FALSE(pc.p_change_state_toggle->get_active()) << "This toggle should not be active, because it has not been pressed yet";

  // Simulate activating the change_status toggle
  pc.p_change_state_toggle->set_active(true);

  // Only the change_status toggle should be pressed
  // We are testing that neither of the toggles can be pressed simultaneously
  ASSERT_FALSE(pc.p_load_profile_toggle->get_active())
    << "This toggle should not be active, because it was not the most recent toggle pressed";

  ASSERT_TRUE(pc.p_change_state_toggle->get_active()) << "This toggle should be active because it was just pressed";

  // Finally untoggle the change_status toggle
  pc.p_change_state_toggle->set_active(false);

  // Neither toggle should be active
  ASSERT_FALSE(pc.p_load_profile_toggle->get_active())
    << "This toggle should not be active, because was not the most recent toggle pressed";

  ASSERT_FALSE(pc.p_change_state_toggle->get_active()) << "This toggle should be active because it was just un-pressed";
}

TEST_F(ProfilesTest, SET_PROFILE_INFO)
{
  std::string logs_str       = "Example string for logs label";
  std::string permission_str = "Example string for permissions label";
  std::string processes_str  = "Example string for processes label";

  pc.set_profile_info(logs_str, permission_str, processes_str);

  ASSERT_EQ(logs_str, std::string(pc.p_num_log_label->get_text()));
  ASSERT_EQ(permission_str, std::string(pc.p_num_perm_label->get_text()));
  ASSERT_EQ(processes_str, std::string(pc.p_num_proc_label->get_text()));
}
