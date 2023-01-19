#include "aa-caller_mock.h"

using ::testing::_;
using ::testing::Return;

class AppArmorCallerTest : public ::testing::Test
{
protected:
  AppArmorCallerTest() = default;
  virtual void SetUp() {}

  AppArmorCallerMock tester;
};

TEST_F(AppArmorCallerTest, TEST_STATUS)
{
  std::vector<std::string> command = { "aa-status", "--json" };
  EXPECT_CALL(tester, call_command(command, _)).Times(1).WillOnce(Return(test_str));

  std::string output = AppArmorCallerMock::get_status(&tester);
  EXPECT_EQ(output, test_str);
}

TEST_F(AppArmorCallerTest, TEST_UNCONF)
{
  std::vector<std::string> command = { "ps", "-A", "--format", "pid,ppid,user,context,comm", "--no-header" };
  EXPECT_CALL(tester, call_command(command, _)).Times(1).WillOnce(Return(test_str));

  std::string output = AppArmorCallerMock::get_unconfined(&tester);
  EXPECT_EQ(output, test_str);
}
