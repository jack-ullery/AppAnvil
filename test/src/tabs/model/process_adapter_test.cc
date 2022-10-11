#include "process_adapter_test.h"
#include "../../../../src/tabs/controller/processes_controller.h"
#include "../../../../src/tabs/model/database.h"
#include "../../../../src/tabs/model/process_adapter.h"
#include "../../../../src/tabs/view/processes.h"
#include "jsoncpp/json/json.h"

#include <cstddef>
#include <gmock/gmock-matchers.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <gtkmm/treeview.h>
#include <gtkmm/window.h>
#include <memory>
#include <regex>
#include <string>
#include <vector>

void ProcessAdapterTest::try_put_data(std::vector<ProcessAdapterTest::TestData> data_set)
{
  for (auto data : data_set) {
    adapter.put_data(data.process_name, data.profile_name, data.pid, data.ppid, data.user, data.status);
  }
}

uint ProcessAdapterTest::entry_count()
{
  uint num_entry = 0;

  for (auto map_pair : database->process_data) {
    auto map = map_pair.second;
    for (auto entry : map) {
      std::ignore = entry;
      num_entry++;
    }
  }

  return num_entry;
}

void ProcessAdapterTest::check_process_entry(ProcessTableEntry entry,
                                             const std::string &process_name,
                                             const std::string &profile_name,
                                             const unsigned int &pid)
{
  ASSERT_EQ(entry.process_name, process_name);
  ASSERT_EQ(entry.profile_name, profile_name);
  ASSERT_EQ(entry.pid, pid);
}

void ProcessAdapterTest::check_put_data(std::vector<ProcessAdapterTest::TestData> data_set, uint num_maps)
{
  ASSERT_TRUE(database->profile_data.empty()) << "We did not add any profile data, so this map should be empty.";
  ASSERT_TRUE(database->log_data.empty()) << "We did not add any log data, so this map should be empty.";
  ASSERT_FALSE(database->process_data.empty()) << "We added " << data_set.size()
                                               << " instances of ProcessEntry, so this map should not be empty.";

  ASSERT_EQ(data_set.size(), entry_count()) << "We expected exactly " << data_set.size() << " entries in the database.";
  ASSERT_EQ(database->process_data.size(), num_maps);

  for (auto data : data_set) {
    // Attempt to retrieve the data
    auto entry_pair     = adapter.get_data(data.profile_name, data.pid);
    auto entry          = entry_pair.first;
    auto did_find_entry = entry_pair.second;

    ASSERT_TRUE(did_find_entry) << "The entry indexed by (" << data.profile_name << ", " << data.pid << ") could not be found";
    check_process_entry(entry, data.process_name, data.profile_name, data.pid);
  }
}

// Test for method put_data
TEST_F(ProcessAdapterTest, PUT_DATA)
{
  TestData data;
  std::vector<TestData> data_set{ data };

  try_put_data(data_set);
  check_put_data(data_set, 1);
}

TEST_F(ProcessAdapterTest, PUT_TWO_PROCESSES_SAME_PROFILE)
{
  TestData data;

  TestData data2;
  data2.pid = 62;

  std::vector<TestData> data_set{ data, data2 };

  try_put_data(data_set);
  check_put_data(data_set, 1);
}

TEST_F(ProcessAdapterTest, PUT_TWO_PROCESSES_DIFFERENT_PROFILES)
{
  TestData data;

  TestData data2;
  data2.pid          = 62;
  data2.profile_name = "alternate_profile";

  std::vector<TestData> data_set{ data, data2 };
  try_put_data(data_set);
  check_put_data(data_set, 2);
}

TEST_F(ProcessAdapterTest, PUT_OVERRIDE_SAME_PROCESS)
{
  TestData data;

  TestData data2;
  data2.process_name = "alternate_process";
  data2.status       = "complain";

  std::vector<TestData> input_data_set{ data, data2 };
  std::vector<TestData> output_data_set{ data2 };

  try_put_data(input_data_set);
  check_put_data(output_data_set, 1);
}
