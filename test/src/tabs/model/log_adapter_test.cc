#include "jsoncpp/json/json.h"
#include "log_adapter_test.h"
#include "../model/status_column_record_mock.h"

#include <gmock/gmock-matchers.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <regex>
#include <string>

// Test Fixture for Logs class
class LogAdapterTest : public ::testing::Test
{
protected:
  LogAdapterTest() 
  : col_record_mock{new StatusColumnRecordMock()},
    database{new DatabaseChild()},
    log_adapter(database, col_record_mock)
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

  // Mock objects
  std::shared_ptr<StatusColumnRecordMock> col_record_mock;
  std::shared_ptr<DatabaseChild> database;

  LogAdapterChild log_adapter;
};

// Test for method format_log_data
TEST_F(LogAdapterTest, TEST_FORMAT_LOG_DATA)
{
  std::string formatted_type      = log_adapter.format_log_data(sample_log_data_type);
  std::string formatted_operation = log_adapter.format_log_data(sample_log_data_operation);
  std::string formatted_status    = log_adapter.format_log_data(sample_log_data_status);

  ASSERT_TRUE(formatted_type.find('\"') == std::string::npos) << "sample type should not contain quotation marks after formatting";
  ASSERT_TRUE(formatted_operation.find('\"') == std::string::npos) << "sample operation should not contain quotation marks after formatting";
  ASSERT_TRUE(formatted_status.find('\"') == std::string::npos) << "sample status should not contain quotation marks after formatting";
  EXPECT_EQ(formatted_type, "STATUS") << "error formatting sample type from log data";
  EXPECT_EQ(formatted_operation, "profile_load") << "error formatting sample operation from log data";
  EXPECT_EQ(formatted_status, "unconfined") << "error formatting sample status from log data";
}

// Test for method format_timestamp
TEST_F(LogAdapterTest, TEST_FORMAT_TIMESTAMP)
{
  std::string formatted_timestamp = log_adapter.format_timestamp(sample_log_data_timestamp);
  std::string formatted_zerotime  = log_adapter.format_timestamp(zerotime);

  bool res = std::regex_match(formatted_timestamp, timestamp_regex);
  ASSERT_TRUE(res) << "formatted timestamp does not match regex";
  res = std::regex_match(formatted_zerotime, timestamp_regex);
  ASSERT_TRUE(res) << "formatted zerotime does not match regex";
}

// // Test for method put_data
// TEST_F(LogAdapterTest, TEST_PUT_DATA)
// {
//   const time_t timestamp = sample_log_data_timestamp;
//   const std::string type = "test_type";
//   const std::string operation = "test_operation";
//   const std::string profile_name = "test_profile_name";
//   const unsigned int pid = 42;
//   const std::string status = "test_status";

//   log_adapter.put_data(timestamp, type, operation, profile_name, pid, status);
//   ASSERT_TRUE(false) << "This test is not completed yet!!!";
// }
