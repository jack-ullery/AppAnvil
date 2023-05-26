#include <filesystem>

#include "profile_modify_test.h"

ProfileModifyTest::ProfileModifyTest()
  : parser{create_parser(filename, temp_filename)},
    profile{get_first_profile(parser)},
    pm(parser, profile)
{   }

ProfileModifyTest::~ProfileModifyTest()
{
  std::filesystem::remove(temp_filename);
}

AppArmor::Parser ProfileModifyTest::create_parser(const std::string &filename, const std::string &temp_filename)
{
  std::filesystem::remove(temp_filename);
  std::filesystem::copy_file(filename, temp_filename);
  return AppArmor::Parser(temp_filename);
}

AppArmor::Profile ProfileModifyTest::get_first_profile(const AppArmor::Parser &parser)
{
  auto profile_list = parser.getProfileList();
  EXPECT_NE(profile_list.size(), 0) << "There should be a profile";
  return profile_list.front();
}

inline AppArmor::Tree::FileRule get_file_rule(const AppArmor::Profile &profile, bool first = true)
{
  auto rule_list = profile.getFileRules();
  EXPECT_NE(rule_list.size(), 0) << "There should be at least one file rule";
  return (first)? rule_list.front() : rule_list.back();
}

TEST_F(ProfileModifyTest, TEST_CONSTRUCTOR)
{
  // This test helps ensure that the fixture runs correctly
  // Otherwise, do nothing...
}

TEST_F(ProfileModifyTest, TEST_DELETE_BUTTON)
{
  std::string button_name = "expected_button_name";
  auto frule = get_file_rule(profile);

  Gtk::Button *button;
  EXPECT_NO_THROW(button = pm.create_delete_button(frule, button_name));
}
