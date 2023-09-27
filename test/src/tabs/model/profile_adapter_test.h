#ifndef TEST_SRC_TABS_MODEL_PROFILE_ADAPTER_TEST_H
#define TEST_SRC_TABS_MODEL_PROFILE_ADAPTER_TEST_H

#include "../../../../src/tabs/controller/profiles_controller.cc"
#include "../../../../src/tabs/model/database.h"
#include "../../../../src/tabs/model/profile_adapter.cc"
#include "../../../../src/tabs/view/profiles.h"
#include "status_column_record_mock.h"

// Test Fixture for Logs class
class ProfileAdapterTest : public ::testing::Test
{
public:
  ProfileAdapterTest()
    : database{ new Database() },
      status{ new Status() },
      adapter(database, status->get_view())
  {
  }

protected:
  // Test data
  struct TestData
  {
    std::string profile_name = "test_profile_name";
    std::string status       = "test_status";
  };

  // Test objects
  std::shared_ptr<Database> database;
  std::shared_ptr<Status> status;
  ProfileAdapter<Database> adapter;

  // Helper methods
  void try_put_data(std::vector<ProfileAdapterTest::TestData> data_set);
  uint entry_count();
  void check_profile_entry(ProfileTableEntry entry, const std::string &profile_name, const std::string &status);
  void check_put_data(std::vector<ProfileAdapterTest::TestData> data_set);
};

#endif // TEST_SRC_TABS_MODEL_PROFILE_ADAPTER_TEST_H