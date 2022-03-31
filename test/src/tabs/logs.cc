#include "logs_mock.cc"
#include "status_column_record_mock.cc"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::_;
using ::testing::Matcher;
using ::testing::Return;
using ::testing::Sequence;

// Test Fixture for Logs class
class LogsTest : public ::testing::Test
{
protected:
  LogsTest() : col_record_mock{new StatusColumnRecordMock()}, row_mock(), logs(col_record_mock) { }

  virtual void SetUp() { }

  // The below strings were obtained by using the command 'dmesg --ctime | grep "audit: type=1400"' in the terminal, which is how appanvil
  // gets the log information from vars/logs/kern.log. This is done to test the methods in logs.inl with realistic sample strings and is
  // necessary for the add_data_to_record method which only calls add_row_to_line when a string that matches filter_regex_log is passed

  std::string line_arg = "[Sun Feb 27 19:15:47 2022] audit: type=1400 audit(1646007318.315:2): apparmor=\"STATUS\" "
                         "operation=\"profile_load\" profile=\"unconfined\" name=\"nvidia_modprobe\" pid=561 comm=\"apparmor_parser\"";
  std::string data_arg =
      "[Sun Feb 27 19:15:47 2022] audit: type=1400 audit(1646007318.315:3): apparmor=\"STATUS\" operation=\"profile_load\" "
      "profile=\"unconfined\" name=\"nvidia_modprobe//kmod\" pid=561 comm=\"apparmor_parser\"\n"
      "[Sun Feb 27 19:15:47 2022] audit: type=1400 audit(1646007318.315:4): apparmor=\"STATUS\" operation=\"profile_load\" "
      "profile=\"unconfined\" name=\"/usr/bin/man\" pid=560 comm=\"apparmor_parser\"";

  int data_arg_num_lines = 2;

  std::shared_ptr<StatusColumnRecordMock> col_record_mock;
  TreeRowMock row_mock;
  LogsMock<StatusColumnRecordMock> logs;
};

// Test for method add_row_from_line(...)
TEST_F(LogsTest, TEST_ADD_ROW_FROM_LINE)
{
  EXPECT_CALL(*col_record_mock, new_row()).Times(1).WillOnce(Return(&row_mock));
  EXPECT_CALL(row_mock, set_value(_, Matcher<const std::string &>(_))).Times(5);
  EXPECT_CALL(row_mock, set_value(_, Matcher<const unsigned long &>(_))).Times(1);

  logs.add_row_from_line(col_record_mock, line_arg);
}

// Test for method add_data_to_record(...)
TEST_F(LogsTest, TEST_ADD_DATA_TO_RECORD)
{
  Sequence add_row_calls;
  EXPECT_CALL(*col_record_mock, clear()).Times(1);
  EXPECT_CALL(*col_record_mock, filter_rows()).Times(1);

  // add_data_to_record calls add_row_from_line(...) for each line that matches the log regex in the passed string
  // with the current values of data_arg and data_arg_num_lines, this means the sequence will occur twice
  for(int i = 0; i < data_arg_num_lines; i++) {
    EXPECT_CALL(*col_record_mock, new_row()).Times(1).InSequence(add_row_calls).WillOnce(Return(&row_mock));
    EXPECT_CALL(row_mock, set_value(_, Matcher<const std::string &>(_))).Times(4).InSequence(add_row_calls);
    EXPECT_CALL(row_mock, set_value(_, Matcher<const unsigned long &>(_))).Times(1).InSequence(add_row_calls);
    EXPECT_CALL(row_mock, set_value(_, Matcher<const std::string &>(_))).Times(1).InSequence(add_row_calls);
  }

  logs.add_data_to_record(data_arg);
}

// Test for method parse_line(...)
TEST_F(LogsTest, TEST_PARSE_LINE)
{
  std::string log_time      = logs.parse_line(line_arg, filter_log_time);
  std::string log_type      = logs.parse_line(line_arg, filter_log_type);
  std::string log_operation = logs.parse_line(line_arg, filter_log_operation);
  std::string log_name      = logs.parse_line(line_arg, filter_log_name);
  std::string log_pid       = logs.parse_line(line_arg, filter_log_pid);
  std::string log_status    = logs.parse_line(line_arg, filter_log_status);

  EXPECT_EQ(log_time, "Sun Feb 27 19:15:47 2022") << "error parsing time from line";
  EXPECT_EQ(log_type, "STATUS") << "error parsing type from line";
  EXPECT_EQ(log_operation, "profile_load") << "error parsing operation from line";
  EXPECT_EQ(log_name, "nvidia_modprobe") << "error parsing name from line";
  EXPECT_EQ(log_pid, "561") << "error parsing pid from line";
  EXPECT_EQ(log_status, "unconfined") << "error parsing status from line";
}

// Test for method refresh()
TEST_F(LogsTest, TEST_REFRESH)
{
  EXPECT_CALL(*col_record_mock, filter_rows()).Times(1);

  logs.refresh();
}
