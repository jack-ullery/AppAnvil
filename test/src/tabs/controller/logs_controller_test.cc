#include "jsoncpp/json/json.h"
#include "logs_controller_test.h"

#include <gmock/gmock-matchers.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>

using ::testing::_;
using ::testing::HasSubstr;
using ::testing::Matcher;
using ::testing::Return;
using ::testing::Sequence;

// Test Fixture for Logs class
class LogsTest : public ::testing::Test
{
protected:
  LogsTest() 
  : col_record_mock{new StatusColumnRecordMock()}, 
    row_mock{new TreeRowMock()}, 
    logs_view{new LogsMock()},
    logs_controller{new LogsControllerChild(col_record_mock, logs_view)}
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
  std::shared_ptr<StatusColumnRecordMock> col_record_mock;
  std::shared_ptr<TreeRowMock> row_mock;
  std::shared_ptr<LogsMock> logs_view;
  std::shared_ptr<LogsControllerChild> logs_controller;
};

// Test for method format_log_data
TEST_F(LogsTest, TEST_FORMAT_LOG_DATA)
{
  std::string formatted_type      = logs_controller->format_log_data(sample_log_data_type);
  std::string formatted_operation = logs_controller->format_log_data(sample_log_data_operation);
  std::string formatted_status    = logs_controller->format_log_data(sample_log_data_status);

  ASSERT_TRUE(formatted_type.find('\"') == std::string::npos) << "sample type should not contain quotation marks after formatting";
  ASSERT_TRUE(formatted_operation.find('\"') == std::string::npos)
      << "sample operation should not contain quotation marks after formatting";
  ASSERT_TRUE(formatted_status.find('\"') == std::string::npos) << "sample status should not contain quotation marks after formatting";
  EXPECT_EQ(formatted_type, "STATUS") << "error formatting sample type from log data";
  EXPECT_EQ(formatted_operation, "profile_load") << "error formatting sample operation from log data";
  EXPECT_EQ(formatted_status, "unconfined") << "error formatting sample status from log data";
}

// Test for method format_timestamp
TEST_F(LogsTest, TEST_FORMAT_TIMESTAMP)
{
  std::string formatted_timestamp = logs_controller->format_timestamp(sample_log_data_timestamp);
  std::string formatted_zerotime  = logs_controller->format_timestamp(zerotime);

  bool res = std::regex_match(formatted_timestamp, timestamp_regex);
  ASSERT_TRUE(res) << "formatted timestamp does not match regex";
  res = std::regex_match(formatted_zerotime, timestamp_regex);
  ASSERT_TRUE(res) << "formatted zerotime does not match regex";
}

// Test for method add_row_from_json
TEST_F(LogsTest, TEST_ADD_ROW_FROM_JSON)
{
  Json::Value root;
  Json::CharReaderBuilder builder;
  JSONCPP_STRING errs;
  std::stringstream stream;
  stream << journalctl_json_snippet;

  bool res = parseFromStream(builder, stream, &root, &errs);
  ASSERT_TRUE(res) << "failed to parse sample json";
  EXPECT_CALL(*col_record_mock, new_row()).Times(1).WillOnce(Return(row_mock.get()));
  EXPECT_CALL(*row_mock, set_value(_, Matcher<const std::string &>(_))).Times(5);
  EXPECT_CALL(*row_mock, set_value(_, Matcher<const unsigned long &>(_))).Times(1);

  logs_controller->add_row_from_json(col_record_mock, root);
}

// Test for method add_data_to_record with a valid argument passed
TEST_F(LogsTest, TEST_ADD_DATA_TO_RECORD_VALID)
{
  EXPECT_CALL(*col_record_mock, clear()).Times(1);
  Sequence add_row_calls;

  // add_data_to_record calls add_row_from_json(...) for each line of the passed json (string)
  // with the current values of data_arg and data_arg_num_lines, this means the sequence will occur twice
  for(int i = 0; i < data_arg_num_lines; i++) {
    EXPECT_CALL(*col_record_mock, new_row()).Times(1).InSequence(add_row_calls).WillOnce(Return(row_mock.get()));
    EXPECT_CALL(*row_mock, set_value(_, Matcher<const std::string &>(_))).Times(4).InSequence(add_row_calls);
    EXPECT_CALL(*row_mock, set_value(_, Matcher<const unsigned long &>(_))).Times(1).InSequence(add_row_calls);
    EXPECT_CALL(*row_mock, set_value(_, Matcher<const std::string &>(_))).Times(1).InSequence(add_row_calls);
  }

  EXPECT_CALL(*col_record_mock, filter_rows()).Times(1);
  EXPECT_CALL(*col_record_mock, reselect_rows()).Times(1);

  logs_controller->add_data_to_record(data_arg);
}

// Test for method add_data_to_record with an invalid argument passed
TEST_F(LogsTest, TEST_ADD_DATA_TO_RECORD_INVALID)
{
  EXPECT_CALL(*col_record_mock, clear()).Times(1);
  EXPECT_CALL(*col_record_mock, new_row()).Times(0);
  EXPECT_CALL(*row_mock, set_value(_, Matcher<const std::string &>(_))).Times(0);
  EXPECT_CALL(*row_mock, set_value(_, Matcher<const unsigned long &>(_))).Times(0);
  EXPECT_CALL(*col_record_mock, filter_rows()).Times(0);
  EXPECT_THROW(logs_controller->add_data_to_record("{test}"), std::invalid_argument);
}

// Test for method refresh()
TEST_F(LogsTest, TEST_REFRESH)
{
  unsigned int arbitrary_num = 1234;
  std::string arbitrary_num_str = std::to_string(1234);

  EXPECT_CALL(*col_record_mock, filter_rows()).Times(1).WillOnce(Return(arbitrary_num));
  EXPECT_CALL(*logs_view, set_status_label_text(HasSubstr(arbitrary_num_str))).Times(1);
  
  logs_controller->refresh();
}
