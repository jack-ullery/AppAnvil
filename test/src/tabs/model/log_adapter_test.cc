#include "log_adapter_test.h"
#include "../model/status_column_record_mock.h"
#include "jsoncpp/json/json.h"
#include "tree_row_mock.h"

#include <gmock/gmock-matchers.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <gtkmm/treestore.h>
#include <regex>
#include <string>

// Test for method format_log_data
TEST_F(LogAdapterTest, TEST_FORMAT_LOG_DATA)
{
  std::string formatted_type      = adapter.format_log_data(sample_log_data_type);
  std::string formatted_operation = adapter.format_log_data(sample_log_data_operation);
  std::string formatted_status    = adapter.format_log_data(sample_log_data_status);

  ASSERT_TRUE(formatted_type.find('\"') == std::string::npos) << "sample type should not contain quotation marks after formatting";
  ASSERT_TRUE(formatted_operation.find('\"') == std::string::npos)
    << "sample operation should not contain quotation marks after formatting";
  ASSERT_TRUE(formatted_status.find('\"') == std::string::npos) << "sample status should not contain quotation marks after formatting";
  EXPECT_EQ(formatted_type, "STATUS") << "error formatting sample type from log data";
  EXPECT_EQ(formatted_operation, "profile_load") << "error formatting sample operation from log data";
  EXPECT_EQ(formatted_status, "unconfined") << "error formatting sample status from log data";
}

// Test for method format_timestamp
TEST_F(LogAdapterTest, TEST_FORMAT_TIMESTAMP)
{
  std::string formatted_timestamp = adapter.format_timestamp(sample_log_data_timestamp);
  std::string formatted_zerotime  = adapter.format_timestamp(zerotime);

  bool res = std::regex_match(formatted_timestamp, timestamp_regex);
  ASSERT_TRUE(res) << "formatted timestamp does not match regex";
  res = std::regex_match(formatted_zerotime, timestamp_regex);
  ASSERT_TRUE(res) << "formatted zerotime does not match regex";
}

void
LogAdapterTest::try_put_data(std::vector<LogAdapterTest::TestData> data_set)
{
  for (auto data : data_set) {
    adapter.put_data(data.timestamp, data.type, data.operation, data.profile_name, data.pid, data.status);
  }
}

uint
LogAdapterTest::entry_count()
{
  uint num_entry = 0;

  for (auto map_pair : database->log_data) {
    auto map = map_pair.second;
    for (auto entry : map) {
      std::ignore = entry;
      num_entry++;
    }
  }

  return num_entry;
}

void
LogAdapterTest::check_log_entry(LogTableEntry entry, TestData expected_data)
{
  ASSERT_EQ(entry.profile_name, expected_data.profile_name);
  ASSERT_EQ(entry.pid, expected_data.pid);
  ASSERT_EQ(entry.timestamp, expected_data.timestamp);
  ASSERT_EQ(entry.type, expected_data.type);
  ASSERT_EQ(entry.operation, expected_data.operation);
}

void
LogAdapterTest::check_put_data(std::vector<LogAdapterTest::TestData> data_set, uint num_maps)
{
  ASSERT_TRUE(database->profile_data.empty()) << "We did not add any profile data, so this map should be empty.";
  ASSERT_TRUE(database->process_data.empty()) << "We did not add any process data, so this map should be empty.";
  ASSERT_FALSE(database->log_data.empty()) << "We added " << data_set.size() << " instances of LogEntry, so this map should not be empty.";

  ASSERT_EQ(data_set.size(), entry_count()) << "We expected exactly " << data_set.size() << " entries in the database.";
  ASSERT_EQ(database->log_data.size(), num_maps);

  for (auto data : data_set) {
    // Attempt to retrieve the data
    auto entry_pair     = adapter.get_data(data.profile_name, data.timestamp);
    auto entry          = entry_pair.first;
    auto did_find_entry = entry_pair.second;

    ASSERT_TRUE(did_find_entry) << "The entry indexed by (" << data.profile_name << ", " << data.pid << ") could not be found";
    check_log_entry(entry, data);
  }
}

// Test for method put_data
TEST_F(LogAdapterTest, PUT_DATA)
{
  TestData data;
  std::vector<TestData> data_set{ data };

  try_put_data(data_set);
  check_put_data(data_set, 1);
}

TEST_F(LogAdapterTest, PUT_TWO_LOGS_SAME_PROFILE)
{
  TestData data;

  TestData data2;
  data2.timestamp = 62;

  std::vector<TestData> data_set{ data, data2 };

  try_put_data(data_set);
  check_put_data(data_set, 1);
}

TEST_F(LogAdapterTest, PUT_TWO_LOGS_DIFFERENT_PROFILES)
{
  TestData data;

  TestData data2;
  data2.profile_name = "alternate_profile";

  std::vector<TestData> data_set{ data, data2 };
  try_put_data(data_set);
  check_put_data(data_set, 2);
}
