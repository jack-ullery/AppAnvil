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

	MOCK_METHOD2(call_command, std::string(std::vector<std::string>, std::string));

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

	std::string output = CommandCallerTest::get_status_str(&tester);
	EXPECT_EQ(output, test_str);
}

TEST_F(COMMAND_CALLER, TEST_LOG){
	std::vector<std::string> command = {"dmesg"};
	EXPECT_CALL(tester, call_command(command, _)).Times(1).WillOnce(Return(test_str));

	std::string output = CommandCallerTest::get_logs_str(&tester);
	EXPECT_EQ(output, test_str);
}

TEST_F(COMMAND_CALLER, TEST_UNCONF){
	std::vector<std::string> command = {"pkexec","aa-unconfined"};
	EXPECT_CALL(tester, call_command(command, _)).Times(1).WillOnce(Return(test_str));

	std::string output = CommandCallerTest::get_unconfined(&tester);
	EXPECT_EQ(output, test_str);
}
