#ifndef TEST_SRC_TABS_CONTROLLER_PROFILES_CONTROLLER_MOCK_TEST_H
#define TEST_SRC_TABS_CONTROLLER_PROFILES_CONTROLLER_MOCK_TEST_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>

// Mock object of Profiles class for use in status and dispatcher_middleman tests
class ProfilesStatusMock
{
public:
  MOCK_METHOD(void, add_data_to_record, (const std::string &data));
  MOCK_METHOD(void, set_apply_label_text, (const std::string &data));
};

#endif