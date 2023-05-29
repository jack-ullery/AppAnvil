#ifndef TEST_SRC_TABS_MODEL_PROCESS_ADAPTER_TEST_H
#define TEST_SRC_TABS_MODEL_PROCESS_ADAPTER_TEST_H

#include "../../../../src/tabs/controller/processes_controller.cc"
#include "../../../../src/tabs/model/database.h"
#include "../../../../src/tabs/model/process_adapter.cc"
#include "../../../../src/tabs/view/processes.h"
#include "status_column_record_mock.h"

// Test Fixture for Process class
class ProcessAdapterTest : public ::testing::Test
{

protected:
  ProcessAdapterTest()
    : database{ new Database() },
      status{ new Status() },
      adapter(database, status->get_view())
  {
  }

protected:
  // Test data
  struct TestData
  {
    std::string process_name = "test_process_name";
    std::string profile_name = "test_profile_name";
    std::string type         = "test_type";
    std::string operation    = "test_operation";
    unsigned int pid         = 42;
    unsigned int ppid        = 12;
    std::string user         = "test_user";
    std::string status       = "test_status";
  };

  // Test objects
  std::shared_ptr<Database> database;
  std::shared_ptr<Status> status;
  ProcessAdapter<Database, StatusColumnRecord> adapter;

  // Helper methods
  void try_put_data(std::vector<ProcessAdapterTest::TestData> data_set);
  uint entry_count();
  void check_process_entry(ProcessTableEntry entry,
                           const std::string &process_name,
                           const std::string &profile_name,
                           const unsigned int &pid);
  void check_put_data(std::vector<ProcessAdapterTest::TestData> data_set, uint num_maps);
};

#endif // TEST_SRC_TABS_MODEL_PROCESS_ADAPTER_TEST_H