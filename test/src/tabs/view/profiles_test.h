#ifndef TEST_SRC_TABS_CONTROLLER_PROFILES_MOCK_TEST_H
#define TEST_SRC_TABS_CONTROLLER_PROFILES_MOCK_TEST_H

#include "../../../../src/tabs/view/profiles.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

class ProfilesChild : public Profiles
{
  FRIEND_TEST(ProfilesTest, APPLY_SIGNAL_HANDLER);
  FRIEND_TEST(ProfilesTest, CHECK_APPLY_LABEL_TEXT);
};

#endif