#include <filesystem>
#include <stdexcept>

#include "profile_modify_test.h"

ProfileModifyTest::ProfileModifyTest()
  : parser{ std::make_shared<::testing::StrictMock<AppArmorParserMock>>() },
    profile{ std::make_shared<AppArmor::Profile>("example_profile", AppArmor::Tree::RuleList()) },
    pm(parser, profile)
{
}

TEST_F(ProfileModifyTest, TEST_CONSTRUCTOR)
{
  // This test helps ensure that the fixture runs correctly
  // Otherwise, do nothing...
}

// To fix linker issues
#include "tabs/view/profile_modify.cc"
template class ProfileModifyImpl<AppArmorParserMock>;
