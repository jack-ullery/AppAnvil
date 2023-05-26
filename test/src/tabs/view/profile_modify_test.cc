#include <filesystem>
#include <stdexcept>

#include "profile_modify_test.h"

ProfileModifyTest::ProfileModifyTest()
  : parser{std::make_shared<::testing::StrictMock<AppArmorParserMock>>()},
    profile{std::make_shared<AppArmor::Profile>("example_profile", AppArmor::Tree::RuleList())},
    pm(parser, profile)
{   }

TEST_F(ProfileModifyTest, TEST_CONSTRUCTOR)
{
  // This test helps ensure that the fixture runs correctly
  // Otherwise, do nothing...
}

TEST_F(ProfileModifyTest, TEST_DELETE_BUTTON)
{
  std::string button_name = "expected_button_name";

  // Create the delete button for a FileRule
  Gtk::Button *button;
  ASSERT_NO_THROW(button = pm.create_delete_button(frule, button_name));
  ASSERT_NE(button, nullptr) << "Created button should not be nullptr";

  // Click the button
  // We assume that pm.removeRule() should be called
  EXPECT_CALL(*parser, removeRule(*profile, frule)).Times(1);
  ASSERT_NO_THROW(button->clicked());
}

TEST_F(ProfileModifyTest, TEST_DELETE_BUTTON_WITH_EXCEPTION)
{
  std::string button_name = "expected_button_name";

  // Create the delete button for a FileRule
  Gtk::Button *button;
  ASSERT_NO_THROW(button = pm.create_delete_button(frule, button_name));
  ASSERT_NE(button, nullptr) << "Created button should not be nullptr";

  // Click the button
  // This time pm.removeRule() will throw a std::domain_error exception
  // Ensure that this does not crash the program
  EXPECT_CALL(*parser, removeRule(*profile, frule))
    .Times(1)
    .WillRepeatedly(::testing::Throw(std::domain_error("example exception")));

  ASSERT_NO_THROW(button->clicked());
}

// To fix linker issues
#include "tabs/view/profile_modify.cc"
template class ProfileModifyImpl<AppArmorParserMock>;
