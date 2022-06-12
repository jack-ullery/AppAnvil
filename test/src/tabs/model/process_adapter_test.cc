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
  // ProcessAdapterTest() 
  // : col_record_mock{new StatusColumnRecordMock()},
  //   database{new DatabaseMockMock()},
  //   proc_adapter(database, col_record_mock)
  // { }

  // // Mock objects
  // std::shared_ptr<StatusColumnRecordMock> col_record_mock;
  // std::shared_ptr<DatabaseMockMock> database;

  // ProcessAdapterChild proc_adapter;
};

// Test for method put_data
TEST_F(ProcessAdapterTest, TEST_PUT_DATA)
{
  std::shared_ptr<DatabaseMock> database{new DatabaseMock()};

  ProcessesController<Processes, DatabaseMock, ProcessAdapter<DatabaseMock, StatusColumnRecord>> processes_controller(database);
  std::shared_ptr<Status> proc{processes_controller.get_tab()};
  
  ProcessAdapter<DatabaseMock, StatusColumnRecord> adapter(database, proc->get_view(), proc->get_window());

  const std::string process_name = "test_process_name";
  const std::string profile_name = "test_profile_name";
  const std::string type = "test_type";
  const std::string operation = "test_operation";
  const unsigned int pid  = 42;
  const unsigned int ppid = 12;
  const std::string user  = "test_user";
  const std::string status = "test_status";

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

  // auto entry_pair = adapter.get_data(process_name, pid);

  // ASSERT_TRUE(entry_pair.second) << "Data should be retrievable after it is inserted into the db";

  // auto entry = entry_pair.first;
  // ASSERT_EQ(entry.pid, pid);
  // ASSERT_EQ(entry.process_name, process_name);
  // ASSERT_EQ(entry.profile_name, profile_name);
  // // ASSERT_NE(entry.row, NULL) << "ProcessAdapter.put_data(..) should not input an empty row into the ColumnRecord";
}
