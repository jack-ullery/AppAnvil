#include "log_adapter_test.h"
#include "../model/status_column_record_mock.h"
#include "tree_row_mock.h"

#include <gmock/gmock-matchers.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <gtkmm/treestore.h>
#include <regex>
#include <string>

// Test for method format_timestamp
TEST_F(LogAdapterTest, TEST_FORMAT_TIMESTAMP)
{
  std::string formatted_timestamp = adapter.format_timestamp(sample_log_data_timestamp);
  std::string formatted_zerotime  = adapter.format_timestamp(zerotime);

  bool res = std::regex_match(formatted_timestamp, timestamp_regex);
  ASSERT_TRUE(res) << "formatted timestamp does not match regex";
  ASSERT_EQ(formatted_zerotime, "Unknown") << "if 'timestamp == 0', then assume we do not know when the log was made";
}

void LogAdapterTest::try_put_data(std::vector<LogAdapterTest::TestData> data_set)
{
  for (auto data : data_set) {
    adapter.put_data(data.timestamp, data.type, data.operation, data.profile_name, data.pid, data.metadata);
  }
}

void LogAdapterTest::check_put_data(std::vector<LogAdapterTest::TestData> data_set, uint num_profs)
{
  ASSERT_TRUE(database->profile_data.empty()) << "We did not add any profile data, so this map should be empty.";
  ASSERT_TRUE(database->process_data.empty()) << "We did not add any process data, so this map should be empty.";
  ASSERT_FALSE(database->log_data.empty()) << "We added " << data_set.size() << " instances of LogEntry, so this map should not be empty.";

  ASSERT_EQ(database->log_data.size(), num_profs);

  for (auto data : data_set) {
    // Retrieve the number of logs for this profile
    uint num_logs = database->get_number_logs(data.profile_name);
    ASSERT_GT(num_logs, 0) << "We expected at least one log for profile: " << data.profile_name;
  }

  uint total_num_logs = 0;
  for (auto log_pair : database->log_data) {
    total_num_logs += log_pair.second;
  }

  ASSERT_EQ(data_set.size(), total_num_logs) << "We expected there to be " << data_set.size()
                                             << " logs recorded in the database, but a count returned " << total_num_logs << " logs.";
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
