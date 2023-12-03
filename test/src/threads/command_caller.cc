#include "command_caller.h"

#include <gtest/gtest.h>

using ::testing::_;
using ::testing::Return;

TEST_F(CommandCallerTest, TEST_STATUS)
{
  std::vector<std::string> command = { "pkexec", "aa-caller", "-s" };
  EXPECT_CALL(tester, call_command(command)).Times(1).WillOnce(Return(tester.result_success));

  auto output = CommandCallerMock::get_status(&tester);
  EXPECT_EQ(output.first, tester.result_success.output);
  EXPECT_TRUE(output.second);
}

TEST_F(CommandCallerTest, TEST_UNCONF)
{
  std::vector<std::string> command = { "pkexec", "aa-caller", "-u" };
  EXPECT_CALL(tester, call_command(command)).Times(1).WillOnce(Return(tester.result_success));

  auto output = CommandCallerMock::get_unconfined(&tester);
  EXPECT_EQ(output.first, tester.result_success.output);
  EXPECT_TRUE(output.second);
}

TEST_F(CommandCallerTest, TEST_LOGS)
{
  std::string filename = "/dev/null";
  std::vector<std::string> command = { "pkexec", "aa-caller", "-l", filename};
  EXPECT_CALL(tester, call_command(command)).Times(1).WillOnce(Return(tester.result_success));

  auto output = CommandCallerMock::get_logs(&tester, filename);
  EXPECT_EQ(output.first, tester.result_success.output);
  EXPECT_TRUE(output.second);
}

TEST_F(CommandCallerTest, TEST_STATUS_PKEXEC_FAIL)
{
  std::vector<std::string> command = { "pkexec", "aa-caller", "-s" };
  EXPECT_CALL(tester, call_command(command)).Times(1).WillOnce(Return(tester.pkexec_error));

  auto output = CommandCallerMock::get_status(&tester);
  EXPECT_FALSE(output.second);
}

TEST_F(CommandCallerTest, TEST_UNCONF_PKEXEC_FAIL)
{
  std::vector<std::string> command = { "pkexec", "aa-caller", "-u" };
  EXPECT_CALL(tester, call_command(command)).Times(1).WillOnce(Return(tester.pkexec_error));

  auto output = CommandCallerMock::get_unconfined(&tester);
  EXPECT_FALSE(output.second);
}

TEST_F(CommandCallerTest, TEST_LOGS_PKEXEC_FAIL)
{
  std::string filename = "/dev/null";
  std::vector<std::string> command = { "pkexec", "aa-caller", "-l", filename};
  EXPECT_CALL(tester, call_command(command)).Times(1).WillOnce(Return(tester.pkexec_error));

  auto output = CommandCallerMock::get_logs(&tester, filename);
  EXPECT_FALSE(output.second);
}

TEST_F(CommandCallerTest, TEST_CHANGE_STATUS_EE)
{
  std::string profile    = "profile";
  std::string old_status = "enforce";
  std::string new_status = "enforce";

  std::string output          = CommandCallerMock::execute_change(&tester, profile, old_status, new_status);
  std::string expected_output = "'" + profile + "' already set to " + old_status + ".";
  EXPECT_EQ(output, expected_output);
}

TEST_F(CommandCallerTest, TEST_CHANGE_STATUS_CC)
{
  std::string profile    = "profile";
  std::string old_status = "complain";
  std::string new_status = "complain";

  std::string output          = CommandCallerMock::execute_change(&tester, profile, old_status, new_status);
  std::string expected_output = "'" + profile + "' already set to " + old_status + ".";
  EXPECT_EQ(output, expected_output);
}

TEST_F(CommandCallerTest, TEST_CHANGE_STATUS_EC_SUCCESS)
{
  std::string profile    = "profile";
  std::string old_status = "enforce";
  std::string new_status = "complain";
  EXPECT_CALL(tester, call_command(_)).WillOnce(Return(tester.result_success));

  std::string output          = CommandCallerMock::execute_change(&tester, profile, old_status, new_status);
  std::string expected_output = " Changed '" + profile + "' from " + old_status + " to " + new_status;
  EXPECT_EQ(output, expected_output);
}

TEST_F(CommandCallerTest, TEST_CHANGE_STATUS_CE_FAIL)
{
  std::string profile    = "profile";
  std::string old_status = "complain";
  std::string new_status = "enforce";
  EXPECT_CALL(tester, call_command(_)).WillOnce(Return(tester.result_error));

  std::string output = CommandCallerMock::execute_change(&tester, profile, old_status, new_status);
  // expect that the output contains the word "Error" somewhere
  EXPECT_TRUE(output.find("Error") != std::string::npos) << "returned string should contain \"Error\" somewhere.";
  // expect that the output contains the error string from the result somewhere
  EXPECT_TRUE(output.find(tester.result_error.error) != std::string::npos)
    << "returned string should contain the error returned from `call_command`.";
}
