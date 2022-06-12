#include "jsoncpp/json/json.h"
#include "process_adapter_test.h"
// #include "../model/status_column_record_mock.h"
#include "../../../../src/tabs/controller/processes_controller.h"
#include "../../../../src/tabs/model/database.h"
#include "../../../../src/tabs/model/process_adapter.h"
#include "../../../../src/tabs/view/processes.h"

#include <cstddef>
#include <gmock/gmock-matchers.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>
#include <regex>
#include <string>

// Test Fixture for Logs class
class ProcessAdapterTest : public ::testing::Test
{
protected:
  ProcessAdapterTest() 
  : database{new DatabaseMock()},
    processes_controller(database),
    proc{processes_controller.get_tab()},
    adapter(database, proc->get_view(), proc->get_window())
  { }

  // Test objects
  std::shared_ptr<DatabaseMock> database;
  ProcessesController<Processes, DatabaseMock, ProcessAdapter<DatabaseMock, StatusColumnRecord>> processes_controller;
  std::shared_ptr<Status> proc;
  ProcessAdapter<DatabaseMock, StatusColumnRecord> adapter;

  // Test data
  const std::string process_name = "test_process_name";
  const std::string profile_name = "test_profile_name";
  const std::string type = "test_type";
  const std::string operation = "test_operation";
  const unsigned int pid  = 42;
  const unsigned int ppid = 12;
  const std::string user  = "test_user";
  const std::string status = "test_status";
};

void check_process_entry(ProcessTableEntry entry, const std::string &process_name, const std::string &profile_name, const unsigned int &pid)
{
  ASSERT_EQ(entry.process_name, process_name);
  ASSERT_EQ(entry.profile_name, profile_name);
  ASSERT_EQ(entry.pid, pid);
}

// Test for method put_data
TEST_F(ProcessAdapterTest, PUT_DATA)
{
  adapter.put_data(process_name, profile_name, pid, ppid, user, status);

  ASSERT_TRUE(database->profile_data.empty())  << "We did not add any profile data, so this map should be empty.";
  ASSERT_TRUE(database->log_data.empty())      << "We did not add any log data, so this map should be empty.";
  ASSERT_FALSE(database->process_data.empty()) << "We added one ProcessEntry, so this map should not be empty.";

  ASSERT_EQ(database->process_data.size(), 1) << "We added one ProcessEntry, so this map should contain one element.";

  auto key_pair = database->process_data.begin();
  ASSERT_EQ(key_pair->first, profile_name) << "The key for the added element should be the profile_name";

  auto pid_map = key_pair->second;
  ASSERT_FALSE(pid_map.empty()) << "We added one ProcessEntry with a profile that points to this map, so it should not be empty.";
  ASSERT_EQ(pid_map.size(), 1) << "We added one ProcessEntry with a profile that points to this map, so it should contain one element.";

  // Check the added entry
  auto entry_pair = pid_map.begin();
  ASSERT_EQ(entry_pair->first, pid) << "The key should be the pid";
  check_process_entry(entry_pair->second, process_name, profile_name, pid);
}

TEST_F(ProcessAdapterTest, PUT_TWO_PROCESSES_SAME_PROFILE)
{
  const unsigned int alternate_pid = 62;

  adapter.put_data(process_name, profile_name, pid, ppid, user, status);
  adapter.put_data(process_name, profile_name, alternate_pid, ppid, user, status);

  ASSERT_TRUE(database->profile_data.empty())  << "We did not add any profile data, so this map should be empty.";
  ASSERT_TRUE(database->log_data.empty())      << "We did not add any log data, so this map should be empty.";
  ASSERT_FALSE(database->process_data.empty()) << "We added two entries, so this map should not be empty.";

  ASSERT_EQ(database->process_data.size(), 1) << "We added two entries using the same profile, so this map should contain one element.";

  auto key_pair = database->process_data.begin();
  ASSERT_EQ(key_pair->first, profile_name) << "The key for the added element should be the profile_name";

  auto pid_map = key_pair->second;
  ASSERT_FALSE(pid_map.empty()) << "We added two entries with a profile that points to this map, so it should not be empty.";
  ASSERT_EQ(pid_map.size(), 2) << "We added two entries with a profile that points to this map, so it should contain two elements.";

  // Need to check the added entries
}

TEST_F(ProcessAdapterTest, PUT_TWO_PROCESSES_DIFFERENT_PROFILES)
{
  const unsigned int alternate_pid = 62;
  const std::string alternate_profile = "alternate_profile";

  adapter.put_data(process_name, profile_name, pid, ppid, user, status);
  adapter.put_data(process_name, alternate_profile, alternate_pid, ppid, user, status);

  ASSERT_TRUE(database->profile_data.empty())  << "We did not add any profile data, so this map should be empty.";
  ASSERT_TRUE(database->log_data.empty())      << "We did not add any log data, so this map should be empty.";
  ASSERT_FALSE(database->process_data.empty()) << "We added one ProcessEntry, so this map should not be empty.";

  ASSERT_EQ(database->process_data.size(), 2) << "We added two entries different profiles, so this map should contain two elements.";

  for(auto name : {profile_name, alternate_profile}){
    auto key_pair = database->process_data.find(name);

    ASSERT_FALSE(key_pair == database->process_data.end()) << "This key should exist in the database.";
    ASSERT_EQ(key_pair->first, name) << "The key for this should be:" + name;

    auto pid_map = key_pair->second;
    ASSERT_FALSE(pid_map.empty()) << "We added one entry with this profile, so it should not be empty.";
    ASSERT_EQ(pid_map.size(), 1) << "We added one entry with this profile, so it should contain one element.";

    // // Check the added entry
    // auto entry_pair = pid_map.begin();
    // ASSERT_EQ(entry_pair->first, pid) << "The key should be the pid";
    // check_process_entry(entry_pair->second, process_name, profile_name, pid);
  }
}

TEST_F(ProcessAdapterTest, PUT_OVERRIDE_SAME_PROCESS)
{
  adapter.put_data(process_name, profile_name, pid, ppid, user, status);
  adapter.put_data(process_name, profile_name, pid, ppid, user, status);

  ASSERT_TRUE(database->profile_data.empty())  << "We did not add any profile data, so this map should be empty.";
  ASSERT_TRUE(database->log_data.empty())      << "We did not add any log data, so this map should be empty.";
  ASSERT_FALSE(database->process_data.empty()) << "We added two entries, so this map should not be empty.";

  ASSERT_EQ(database->process_data.size(), 1) << "We added two entries using the same profile, so this map should contain one element.";

  auto key_pair = database->process_data.begin();
  ASSERT_EQ(key_pair->first, profile_name) << "The key for the added element should be the profile_name";

  auto pid_map = key_pair->second;
  ASSERT_FALSE(pid_map.empty()) << "We added at least one entry with a profile that points to this map, so it should not be empty.";
  ASSERT_EQ(pid_map.size(), 1) << "We overwrote an entry, so this map should contain one element.";

  // Check the added entry
  auto entry_pair = pid_map.begin();
  ASSERT_EQ(entry_pair->first, pid) << "The key should be the pid";
  check_process_entry(entry_pair->second, process_name, profile_name, pid);
}

