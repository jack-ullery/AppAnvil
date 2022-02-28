#include "logs_mock.cc"
#include "status_column_record_mock.cc"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::Sequence;

class LogsTest : public ::testing::Test
{
protected:
  LogsTest() : col_record_mock{new StatusColumnRecordMock()}, logs(col_record_mock)
  {
  }

  virtual void SetUp() { }

  std::string line_arg = "[Sun Feb 27 19:15:47 2022] audit: type=1400 audit(1646007318.315:2): apparmor=\"STATUS\" "
                         "operation=\"profile_load\" profile=\"unconfined\" name=\"nvidia_modprobe\" pid=561 comm=\"apparmor_parser\"";
  std::string data_arg =
      "[Sun Feb 27 19:15:47 2022] audit: type=1400 audit(1646007318.315:3): apparmor=\"STATUS\" operation=\"profile_load\" "
      "profile=\"unconfined\" name=\"nvidia_modprobe//kmod\" pid=561 comm=\"apparmor_parser\"\n"
      "[Sun Feb 27 19:15:47 2022] audit: type=1400 audit(1646007318.315:4): apparmor=\"STATUS\" operation=\"profile_load\" "
      "profile=\"unconfined\" name=\"/usr/bin/man\" pid=560 comm=\"apparmor_parser\"";

  int data_arg_num_lines = 2;

  std::shared_ptr<StatusColumnRecordMock> col_record_mock;
  LogsMock<StatusColumnRecordMock> logs;
};

TEST_F(LogsTest, TEST_ADD_ROW_FROM_LINE) 
{ 
  EXPECT_CALL(*col_record_mock, new_row()).Times(1);
  EXPECT_CALL(*col_record_mock, set_row_data).Times(6);

  logs.add_row_from_line(col_record_mock, line_arg);
}

TEST_F(LogsTest, TEST_ADD_DATA_TO_RECORD)
{
  Sequence add_row_calls;
  EXPECT_CALL(*col_record_mock, clear()).Times(1);
  EXPECT_CALL(*col_record_mock, filter_rows()).Times(1);

  for (int i = 0; i < data_arg_num_lines; i++) {
    EXPECT_CALL(*col_record_mock, new_row()).Times(1).InSequence(add_row_calls);
    EXPECT_CALL(*col_record_mock, set_row_data).Times(6).InSequence(add_row_calls);
  }
  
  logs.add_data_to_record(data_arg);
}

TEST_F(LogsTest, TEST_PARSE_LINE) 
{ 
  std::string log_time      = logs.parse_line(line_arg, filter_log_time);
  std::string log_type      = logs.parse_line(line_arg, filter_log_type<StatusColumnRecordMock>);
  std::string log_operation = logs.parse_line(line_arg, filter_log_operation<StatusColumnRecordMock>);
  std::string log_name      = logs.parse_line(line_arg, filter_log_name<StatusColumnRecordMock>);
  std::string log_pid       = logs.parse_line(line_arg, filter_log_pid);
  std::string log_status    = logs.parse_line(line_arg, filter_log_status<StatusColumnRecordMock>);

  EXPECT_EQ(log_time, "Sun Feb 27 19:15:47 2022") << "error parsing time from line";
  EXPECT_EQ(log_type, "STATUS") << "error parsing type from line";
  EXPECT_EQ(log_operation, "profile_load") << "error parsing operation from line";
  EXPECT_EQ(log_name, "nvidia_modprobe") << "error parsing name from line";
  EXPECT_EQ(log_pid, "561") << "error parsing pid from line";
  EXPECT_EQ(log_status, "unconfined") << "error parsing status from line";
}

TEST_F(LogsTest, TEST_REFRESH)
{ 
  EXPECT_CALL(*col_record_mock, filter_rows()).Times(1);

  logs.refresh();
}
