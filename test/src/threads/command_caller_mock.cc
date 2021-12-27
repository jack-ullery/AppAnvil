#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "../../../src/threads/command_caller.h"
#include <vector>

const std::string test_str = "the quick brown fox jumps over the lazy dog"; 
const std::string success_str = "success"; 
const std::string error_str = "error"; 

class CommandCallerMock : public CommandCaller
{
public:
	FRIEND_TEST(CommandCallerTest, TEST_UNCONF);
	FRIEND_TEST(CommandCallerTest, TEST_LOG);
	FRIEND_TEST(CommandCallerTest, TEST_STATUS);
	FRIEND_TEST(CommandCallerTest, TEST_CHANGE_STATUS_EE);
	FRIEND_TEST(CommandCallerTest, TEST_CHANGE_STATUS_CC);
	FRIEND_TEST(CommandCallerTest, TEST_CHANGE_STATUS_CE);
	FRIEND_TEST(CommandCallerTest, TEST_CHANGE_STATUS_EC_SUCCESS);
	FRIEND_TEST(CommandCallerTest, TEST_CHANGE_STATUS_CE_FAIL);

	MOCK_METHOD(std::string, call_command, (const std::vector<std::string>&, const std::string&), (override));
	MOCK_METHOD(CommandCaller::results, call_command, (const std::vector<std::string>&), (override));

	CommandCallerMock::results result_success = {0, success_str, error_str};
	CommandCallerMock::results result_error = {1, success_str, error_str};
};
