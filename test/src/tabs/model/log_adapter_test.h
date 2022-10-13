#ifndef TEST_SRC_TABS_MODEL_LOG_ADAPTER_TEST_H
#define TEST_SRC_TABS_MODEL_LOG_ADAPTER_TEST_H

#include "../../../../src/tabs/entries.h"
#include "../../../../src/tabs/model/log_adapter.cc"
#include "status_column_record_mock.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

// Test Fixture for Logs class
class LogAdapterTest : public ::testing::Test
{
protected:
  LogAdapterTest()
    : database{ new Database() },
      status{ new Status() },
      adapter(database, status->get_view(), status->get_window())
  {
  }

  // Some of the sample snippets of information below were taken from the output of running 'journalctl -b "_AUDIT_TYPE=1400" --output=json'
  // in the terminal. They are mostly used to verify that information is being formatted correctly in format_log_data and format_timestamp.
  // A string in valid json format is also necessary to test the functionality of add_row_from_json and add_data_to_record as they will
  // throw an exception when trying to parse the json data otherwise.
  std::string sample_log_data_type      = "\"STATUS\"";
  std::string sample_log_data_operation = "\"profile_load\"";
  std::string sample_log_data_status    = "\"unconfined\"";
  time_t sample_log_data_timestamp      = (time_t)1648234140; // Fri Mar 25 14:49:00 2022 (time as of writing tests (Eastern Time))
  time_t zerotime                       = (time_t)0;          // Thu Jan 1st 00:00:00 1970 (UTC) (Wed Dec 31 19:00:00 1969 in Eastern Time)
  std::regex timestamp_regex            = std::regex("\\d{4}-\\d{2}-\\d{2}\\s{1}\\d{2}:\\d{2}:\\d{2}\\t");

  struct TestData
  {
    std::string profile_name = "test_profile_name";
    unsigned int pid         = 42;
    time_t timestamp         = (time_t)1648234140;
    std::string type         = "\"STATUS\"";
    std::string operation    = "\"profile_load\"";
    std::list<std::pair<std::string, std::string>> metadata;
  };

  // Test objects
  std::shared_ptr<Database> database;
  std::shared_ptr<Status> status;
  LogAdapter<Database, StatusColumnRecord> adapter;

  // Helper methods
  void try_put_data(std::vector<LogAdapterTest::TestData> data_set);
  void check_put_data(std::vector<LogAdapterTest::TestData> data_set, uint num_maps);
};

#endif // TEST_SRC_TABS_MODEL_LOG_ADAPTER_TEST_H