#include "jsoncpp/json/json.h"
#include "logs_controller_test.h"
#include "../model/status_column_record_mock.h"
#include "../model/tree_row_mock.h"

#include <gmock/gmock-matchers.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>

using ::testing::_;
using ::testing::HasSubstr;
using ::testing::Return;
using ::testing::Sequence;

// Test Fixture for Logs class
class LogsControllerTest : public ::testing::Test
{
protected:
  LogsControllerTest() 
  : adapter_mock{new LogAdapterMock()}, 
    col_record_mock{new StatusColumnRecordMock()},
    logs_view_mock{new LogsMock()},
    logs_controller{new LogsControllerChild(adapter_mock, logs_view_mock)}
  { }

  // Some of the sample snippets of information below were taken from the output of running 'journalctl -b "_AUDIT_TYPE=1400" --output=json'
  // in the terminal. They are mostly used to verify that information is being formatted correctly in format_log_data and format_timestamp.
  // A string in valid json format is also necessary to test the functionality of add_row_from_json and add_data_to_record as they will
  // throw an exception when trying to parse the json data otherwise.
  std::string sample_log_data_type      = "\"STATUS\"";
  std::string sample_log_data_operation = "\"profile_load\"";
  std::string sample_log_data_status    = "\"unconfined\"";
  time_t sample_log_data_timestamp      = (time_t) 1648234140; // Fri Mar 25 14:49:00 2022 (time as of writing tests (Eastern Time))
  time_t zerotime                       = (time_t) 0;          // Thu Jan 1st 00:00:00 1970 (UTC) (Wed Dec 31 19:00:00 1969 in Eastern Time)
  std::regex timestamp_regex            = std::regex("\\d{4}-\\d{2}-\\d{2}\\s{1}\\d{2}:\\d{2}:\\d{2}\\t");
  std::string journalctl_json_snippet =
      "{\"_SOURCE_REALTIME_TIMESTAMP\" : \"1648231725959000\",\"_AUDIT_FIELD_APPARMOR\" : \"\\\"STATUS\\\"\","
      "\"_AUDIT_FIELD_OPERATION\" : \"\\\"profile_load\\\"\",\"_AUDIT_FIELD_NAME\" : \"nvidia_modprobe\","
      "\"_PID\" : \"601\",\"_AUDIT_FIELD_PROFILE\" : \"\\\"unconfined\\\"\"}";
  int data_arg_num_lines = 2;
  std::string data_arg   = "{\"_SOURCE_REALTIME_TIMESTAMP\" : \"1648231725959000\",\"_AUDIT_FIELD_APPARMOR\" : \"\\\"STATUS\\\"\","
                         "\"_AUDIT_FIELD_OPERATION\" : \"\\\"profile_load\\\"\",\"_AUDIT_FIELD_NAME\" : \"nvidia_modprobe\","
                         "\"_PID\" : \"601\",\"_AUDIT_FIELD_PROFILE\" : \"\\\"unconfined\\\"\"}\n"
                         "{\"_SOURCE_REALTIME_TIMESTAMP\" : \"1648231725959000\",\"_AUDIT_FIELD_APPARMOR\" : \"\\\"STATUS\\\"\","
                         "\"_AUDIT_FIELD_OPERATION\" : \"\\\"profile_load\\\"\",\"_AUDIT_FIELD_NAME\" : \"nvidia_modprobe\","
                         "\"_PID\" : \"601\",\"_AUDIT_FIELD_PROFILE\" : \"\\\"unconfined\\\"\"}";

  // Mock objects
  std::shared_ptr<LogAdapterMock> adapter_mock;
  std::shared_ptr<StatusColumnRecordMock> col_record_mock;
  std::shared_ptr<LogsMock> logs_view_mock;
  std::shared_ptr<LogsControllerChild> logs_controller;
};

// Test for method add_row_from_json
TEST_F(LogsControllerTest, TEST_ADD_ROW_FROM_JSON)
{
  Json::Value root;
  Json::CharReaderBuilder builder;
  JSONCPP_STRING errs;
  std::stringstream stream;
  stream << journalctl_json_snippet;

  bool res = parseFromStream(builder, stream, &root, &errs);
  ASSERT_TRUE(res) << "failed to parse sample json";

  // Should check that the arguments are correct
  EXPECT_CALL(*adapter_mock, put_data(_, _, _, _, _, _))
    .Times(1);

  logs_controller->add_row_from_json(root);
}

// Test for method add_data_to_record with a valid argument passed
TEST_F(LogsControllerTest, TEST_ADD_DATA_TO_RECORD_VALID)
{
  uint arbitrary_num = 42;
  Sequence add_row_calls;

  // add_data_to_record calls add_row_from_json(...) for each line of the passed json (string)
  // with the current values of data_arg and data_arg_num_lines, this means the sequence will occur twice
  for(int i = 0; i < data_arg_num_lines; i++) {
    EXPECT_CALL(*adapter_mock, put_data(_, _, _, _, _, _))
      .Times(1)
      .InSequence(add_row_calls);
  }

  EXPECT_CALL(*adapter_mock, get_col_record())
    .Times(1)
    .InSequence(add_row_calls)
    .WillOnce(Return(col_record_mock));

  EXPECT_CALL(*col_record_mock, filter_rows())
    .Times(1)
    .WillOnce(Return(arbitrary_num));

  EXPECT_CALL(*logs_view_mock, set_status_label_text(::HasSubstr(std::to_string(arbitrary_num) + " logs")))
    .Times(1)
    .InSequence(add_row_calls);

  logs_controller->add_data_to_record(data_arg);
}

// Test for method add_data_to_record with an invalid argument passed
TEST_F(LogsControllerTest, TEST_ADD_DATA_TO_RECORD_INVALID)
{
  EXPECT_CALL(*adapter_mock, put_data(_, _, _, _, _, _)).Times(0);
  EXPECT_CALL(*adapter_mock, get_col_record()).Times(0);

  EXPECT_THROW(logs_controller->add_data_to_record("{test}"), std::invalid_argument);
}

// Test for method refresh()
TEST_F(LogsControllerTest, TEST_REFRESH)
{
  Sequence refresh_sequence;
  unsigned int arbitrary_num = 1234;

  EXPECT_CALL(*adapter_mock, get_col_record())
    .Times(1)
    .InSequence(refresh_sequence)
    .WillOnce(Return(col_record_mock));

  EXPECT_CALL(*col_record_mock, filter_rows())
    .Times(1)
    .WillOnce(Return(arbitrary_num));

  EXPECT_CALL(*logs_view_mock, set_status_label_text(::HasSubstr(std::to_string(arbitrary_num))))
    .Times(1)
    .InSequence(refresh_sequence);

  logs_controller->refresh();
}
