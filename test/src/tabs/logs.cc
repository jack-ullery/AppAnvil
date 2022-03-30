#include "logs_mock.cc"
#include "status_column_record_mock.cc"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::_;

// Test Fixture for Logs class
class LogsTest : public ::testing::Test
{
protected:
  LogsTest() : col_record_mock{new StatusColumnRecordMock()}, logs(col_record_mock) { }

  virtual void SetUp() { }

  // Some of the sample snippets of information below were taken from the output of running 'journalctl -b "_AUDIT_TYPE=1400" --output=json' 
  // in the terminal. They are mostly used to verify that information is being formatted correctly in format_log_data and format_timestamp.
  // A string in valid json format is also necessary to test the functionality of add_row_from_json and add_data_to_record as they will 
  // throw an exception when trying to parse the json data otherwise.
  std::string sample_log_data_type      = "\"STATUS\"";
  std::string sample_log_data_operation = "\"profile_load\"";
  std::string sample_log_data_status    = "\"unconfined\"";
  time_t sample_log_data_timestamp      = (time_t) 1648234140;  // Fri Mar 25 14:49:00 2022 (time as of writing tests (Eastern Time))
  time_t zerotime                       = (time_t) 0;           // Thu Jan 1st 00:00:00 1970 (UTC) (Wed Dec 31 19:00:00 1969 in Eastern Time)
  std::regex timestamp_regex            = std::regex("\\d{4}-\\d{2}-\\d{2}\\s{1}\\d{2}:\\d{2}:\\d{2}\\t");
  std::string journalctl_json_snippet   = "{\"_SOURCE_REALTIME_TIMESTAMP\" : \"1648231725959000\",\"_AUDIT_FIELD_APPARMOR\" : \"\\\"STATUS\\\"\","
                                       "\"_AUDIT_FIELD_OPERATION\" : \"\\\"profile_load\\\"\",\"_AUDIT_FIELD_NAME\" : \"nvidia_modprobe\","
                                       "\"_PID\" : \"601\",\"_AUDIT_FIELD_PROFILE\" : \"\\\"unconfined\\\"\"}";

  // Mock objects
  std::shared_ptr<StatusColumnRecordMock> col_record_mock;
  LogsMock<StatusColumnRecordMock> logs;
};

// Test for method format_log_data
TEST_F(LogsTest, TEST_FORMAT_LOG_DATA) 
{ 
  std::string formatted_type      = logs.format_log_data(sample_log_data_type);
  std::string formatted_operation = logs.format_log_data(sample_log_data_operation);
  std::string formatted_status    = logs.format_log_data(sample_log_data_status);

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
  std::string formatted_timestamp = logs.format_timestamp(sample_log_data_timestamp);
  std::string formatted_zerotime  = logs.format_timestamp(zerotime);

  bool res = std::regex_match(formatted_timestamp, timestamp_regex);
  ASSERT_TRUE(res) << "formatted timestamp does not match regex";
  res = std::regex_match(formatted_zerotime, timestamp_regex);
  ASSERT_TRUE(res) << "formatted zerotime does not match regex";

  // Note that the expected values are in Eastern Time not in UTC because the timestamp is converted to localtime in format_timestamp
  EXPECT_EQ(formatted_timestamp, "2022-03-25 14:49:00\t") << "error formatting sample timestamp from log data";
  EXPECT_EQ(formatted_zerotime, "1969-12-31 19:00:00\t") << "error formatting zerotime timestamp";
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
  EXPECT_CALL(*col_record_mock, new_row()).Times(1);

  const time_t timestamp = std::stol(root["_SOURCE_REALTIME_TIMESTAMP"].asString()) / 1000000;
  std::string type = root["_AUDIT_FIELD_APPARMOR"].asString();
  std::string operation = root["_AUDIT_FIELD_OPERATION"].asString();
  std::string status    = root["_AUDIT_FIELD_PROFILE"].asString();
  EXPECT_CALL(*col_record_mock, set_row_data(_, 0, logs.format_timestamp(timestamp)));
  EXPECT_CALL(*col_record_mock, set_row_data(_, 1, logs.format_log_data(type)));
  EXPECT_CALL(*col_record_mock, set_row_data(_, 2, logs.format_log_data(operation)));
  EXPECT_CALL(*col_record_mock, set_row_data(_, 3, root["_AUDIT_FIELD_NAME"].asString()));
  EXPECT_CALL(*col_record_mock, set_row_data(_, 4, root["_PID"].asString()));
  EXPECT_CALL(*col_record_mock, set_row_data(_, 5, logs.format_log_data(status)));

  logs.add_row_from_json(col_record_mock, root);
}

// Test for method add_data_to_record with a valid argument passed
TEST_F(LogsTest, TEST_ADD_DATA_TO_RECORD_VALID)
{
  EXPECT_CALL(*col_record_mock, clear()).Times(1);
  EXPECT_CALL(*col_record_mock, new_row()).Times(1);
  EXPECT_CALL(*col_record_mock, set_row_data).Times(6);
  EXPECT_CALL(*col_record_mock, filter_rows()).Times(1);

  logs.add_data_to_record(journalctl_json_snippet);
}

// Test for method add_data_to_record with an invalid argument passed
TEST_F(LogsTest, TEST_ADD_DATA_TO_RECORD_INVALID)
{
  EXPECT_CALL(*col_record_mock, clear()).Times(1);
  EXPECT_CALL(*col_record_mock, new_row()).Times(0);
  EXPECT_CALL(*col_record_mock, set_row_data).Times(0);
  EXPECT_CALL(*col_record_mock, filter_rows()).Times(0);
  EXPECT_THROW(logs.add_data_to_record("{test}"), std::invalid_argument);
}

// Test for method refresh()
TEST_F(LogsTest, TEST_REFRESH)
{
  EXPECT_CALL(*col_record_mock, filter_rows()).Times(1);

  logs.refresh();
}