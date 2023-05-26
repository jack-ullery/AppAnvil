#ifndef TEST_SRC_TABS_VIEW_PROFILE_MODIFY_TEST_H
#define TEST_SRC_TABS_VIEW_PROFILE_MODIFY_TEST_H

#include <gtest/gtest.h>
#include <libappanvil/apparmor_parser.hh>
#include <libappanvil/tree/AbstractionRule.hh>
#include <libappanvil/tree/FileRule.hh>

#include "tabs/view/profile_modify.h"

// Test Fixture for Process class
class ProfileModifyTest : public ::testing::Test
{
  protected:
    ProfileModifyTest();
    ~ProfileModifyTest();

    static AppArmor::Parser create_parser(const std::string &filename, const std::string &temp_filename);
    static AppArmor::Profile get_first_profile(const AppArmor::Parser &parser);

    const std::string filename = EXAMPLE_PROFILE_SOURCE_DIR "/test1.sd";
    const std::string temp_filename = EXAMPLE_PROFILE_SOURCE_DIR "/temp.sd";

    AppArmor::Parser parser;
    AppArmor::Profile profile;
    ProfileModify pm;
};

#endif // TEST_SRC_TABS_VIEW_PROFILE_MODIFY_TEST_H