#ifndef TEST_SRC_THREADS_COMMAND_CALLER_H
#define TEST_SRC_THREADS_COMMAND_CALLER_H

#include "../../../src/threads/command_caller.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <vector>

const std::string test_str    = "the quick brown fox jumps over the lazy dog";
const std::string success_str = "success";
const std::string error_str   = "error";
const std::string pkexec_str  = "Error executing command as another user: Request dismissed";

class CommandCallerMock : public CommandCaller
{
public:
  MOCK_METHOD(std::string, call_command, (const std::vector<std::string> &, const std::string &), (override));
  MOCK_METHOD(CommandCaller::results, call_command, (const std::vector<std::string> &), (override));

  CommandCallerMock::results result_success = { 0, success_str, error_str };
  CommandCallerMock::results result_error   = { 1, test_str, error_str };
  CommandCallerMock::results pkexec_error   = { 1, test_str, pkexec_str };
};

class CommandCallerTest : public ::testing::Test
{
protected:
  CommandCallerTest() = default;
  virtual void SetUp() {}

  CommandCallerMock tester;
};

#endif