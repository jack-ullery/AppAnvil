#ifndef TEST_SRC_TABS_VIEW_PROFILE_MODIFY_TEST_H
#define TEST_SRC_TABS_VIEW_PROFILE_MODIFY_TEST_H

#include <gtest/gtest.h>
#include <libappanvil/apparmor_parser.hh>
#include <libappanvil/tree/AbstractionRule.hh>
#include <libappanvil/tree/FileRule.hh>

#include "../../libs/apparmor_parser_mock.h"
#include "tabs/view/profile_modify.h"

// Test Fixture for Process class
class ProfileModifyTest : public ::testing::Test
{
  protected:
    ProfileModifyTest();

    std::shared_ptr<::testing::StrictMock<AppArmorParserMock>> parser;
    std::shared_ptr<AppArmor::Profile> profile;
    ProfileModifyImpl<AppArmorParserMock> pm;

    // Test values
    AppArmor::Tree::FileRule frule;
};

#endif // TEST_SRC_TABS_VIEW_PROFILE_MODIFY_TEST_H