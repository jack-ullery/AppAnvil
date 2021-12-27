#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <iostream>
#include "../../../src/threads/command_caller.h"
#include <vector>

using ::testing::_;
using ::testing::Return;

const std::string test_str = "the quick brown fox jumps over the lazy dog"; 
const std::string success_str = "success"; 
const std::string error_str = "error"; 

class CommandCallerTest : public CommandCaller
{
public:
	FRIEND_TEST(COMMAND_CALLER, TEST_UNCONF);
	FRIEND_TEST(COMMAND_CALLER, TEST_LOG);
	FRIEND_TEST(COMMAND_CALLER, TEST_STATUS);
	FRIEND_TEST(COMMAND_CALLER, TEST_CHANGE_STATUS_EE);
	FRIEND_TEST(COMMAND_CALLER, TEST_CHANGE_STATUS_CC);
	FRIEND_TEST(COMMAND_CALLER, TEST_CHANGE_STATUS_CE);
	FRIEND_TEST(COMMAND_CALLER, TEST_CHANGE_STATUS_EC_SUCCESS);
	FRIEND_TEST(COMMAND_CALLER, TEST_CHANGE_STATUS_CE_FAIL);

	MOCK_METHOD(std::string, call_command, (const std::vector<std::string>&, const std::string&), (override));
	MOCK_METHOD(CommandCaller::results, call_command, (const std::vector<std::string>&), (override));

	CommandCallerTest::results result_success = {0, success_str, error_str};
	CommandCallerTest::results result_error = {1, success_str, error_str};
};

class COMMAND_CALLER : public ::testing::Test
{
protected:
	COMMAND_CALLER() = default;
	virtual void SetUp(){}
	
	CommandCallerTest tester;
};

TEST_F(COMMAND_CALLER, TEST_STATUS){
	std::vector<std::string> command = {"pkexec","aa-status","--json"};
	EXPECT_CALL(tester, call_command(command, _)).Times(1).WillOnce(Return(test_str));
	EXPECT_CALL(tester, call_command(_)).Times(0);

	std::string output = CommandCallerTest::get_status_str(&tester);
	EXPECT_EQ(output, test_str);
}

TEST_F(COMMAND_CALLER, TEST_LOG){
	std::vector<std::string> command = {"dmesg"};
	EXPECT_CALL(tester, call_command(command, _)).Times(1).WillOnce(Return(test_str));
	EXPECT_CALL(tester, call_command(_)).Times(0);

	std::string output = CommandCallerTest::get_logs_str(&tester);
	EXPECT_EQ(output, test_str);
}

TEST_F(COMMAND_CALLER, TEST_UNCONF){
	std::vector<std::string> command = {"pkexec","aa-unconfined"};
	EXPECT_CALL(tester, call_command(command, _)).Times(1).WillOnce(Return(test_str));
	EXPECT_CALL(tester, call_command(_)).Times(0);

	std::string output = CommandCallerTest::get_unconfined(&tester);
	EXPECT_EQ(output, test_str);
}

TEST_F(COMMAND_CALLER, TEST_CHANGE_STATUS_EE){
	std::string profile = "profile";
	std::string old_status = "enforce";
	std::string new_status = "enforce";
	EXPECT_CALL(tester, call_command(_, _)).Times(0);
	EXPECT_CALL(tester, call_command(_)).Times(0);

	std::string output = CommandCallerTest::execute_change(&tester, profile, old_status, new_status);
	std::string expected_output = "'" + profile + "' already set to " + old_status + ".";
	EXPECT_EQ(output, expected_output);
}

TEST_F(COMMAND_CALLER, TEST_CHANGE_STATUS_CC){
	std::string profile = "profile";
	std::string old_status = "complain";
	std::string new_status = "complain";
	EXPECT_CALL(tester, call_command(_, _)).Times(0);
	EXPECT_CALL(tester, call_command(_)).Times(0);

	std::string output = CommandCallerTest::execute_change(&tester, profile, old_status, new_status);
	std::string expected_output = "'" + profile + "' already set to " + old_status + ".";
	EXPECT_EQ(output, expected_output);
}

TEST_F(COMMAND_CALLER, TEST_CHANGE_STATUS_EC_SUCCESS){
	std::string profile = "profile";
	std::string old_status = "enforce";
	std::string new_status = "complain";
	EXPECT_CALL(tester, call_command(_,_)).Times(0);
	EXPECT_CALL(tester, call_command(_)).WillOnce(Return(tester.result_success));

	std::string output = CommandCallerTest::execute_change(&tester, profile, old_status, new_status);
	std::string expected_output = " Changed '" + profile + "' from " + old_status + " to " + new_status;
	EXPECT_EQ(output, expected_output);
}

TEST_F(COMMAND_CALLER, TEST_CHANGE_STATUS_CE_FAIL){
	std::string profile = "profile";
	std::string old_status = "complain";
	std::string new_status = "enforce";
	EXPECT_CALL(tester, call_command(_,_)).Times(0);
	EXPECT_CALL(tester, call_command(_)).WillOnce(Return(tester.result_error));

	std::string output = CommandCallerTest::execute_change(&tester, profile, old_status, new_status);
	// expect that the output contains the word "Error" somewhere
	EXPECT_TRUE(output.find("Error") != std::string::npos) << "returned string should contain \"Error\" somewhere.";
	// expect that the output contains the error string from the result somewhere
	EXPECT_TRUE(output.find(tester.result_error.error) != std::string::npos)  << "returned string should contain the error returned from `call_command`.";
}
