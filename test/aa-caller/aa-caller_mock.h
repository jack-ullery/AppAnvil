#ifndef TEST_SRC_THREADS_APP_ARMOR_CALLER_MOCK_H
#define TEST_SRC_THREADS_APP_ARMOR_CALLER_MOCK_H

#include "../../aa-caller/src/aa-caller.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <vector>

const std::string test_str    = "the quick brown fox jumps over the lazy dog";
const std::string success_str = "success";
const std::string error_str   = "error";

class AppArmorCallerMock : public AppArmorCaller
{
public:
  FRIEND_TEST(AppArmorCallerTest, TEST_UNCONF);
  FRIEND_TEST(AppArmorCallerTest, TEST_LOG);
  FRIEND_TEST(AppArmorCallerTest, TEST_STATUS);

  MOCK_METHOD(std::string, call_command, (const std::vector<std::string> &, const std::string &), (override));
  MOCK_METHOD(AppArmorCaller::results, call_command, (const std::vector<std::string> &), (override));

  AppArmorCallerMock::results result_success = { 0, success_str, error_str };
  AppArmorCallerMock::results result_error   = { 1, success_str, error_str };
};

#endif