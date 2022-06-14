#include "jsoncpp/json/json.h"
#include "profile_adapter_test.h"
#include "../../../../src/tabs/controller/profiles_controller.h"
#include "../../../../src/tabs/model/database.h"
#include "../../../../src/tabs/model/profile_adapter.h"
#include "../../../../src/tabs/view/profiles.h"

#include <cstddef>
#include <gmock/gmock-matchers.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>
#include <regex>
#include <string>
#include <vector>

// Test Fixture for Logs class
class ProfileAdapterTest : public ::testing::Test
{

protected:
  ProfileAdapterTest() 
  : database{new DatabaseMock()},
    status{new Status()},
    adapter(database, status->get_view(), status->get_window())
  { }

protected:
  // Test data
  struct TestData {
    std::string profile_name = "test_profile_name";
    std::string status = "test_status";
  };

  // Test objects
  std::shared_ptr<DatabaseMock> database;
  std::shared_ptr<Status> status;
  ProfileAdapter<DatabaseMock> adapter;

  // Helper methods
  void try_put_data(std::vector<ProfileAdapterTest::TestData> data_set);
  uint entry_count();
  void check_profile_entry(ProfileTableEntry entry, const std::string &profile_name, const std::string &status);
  void check_put_data(std::vector<ProfileAdapterTest::TestData> data_set);
};

void ProfileAdapterTest::try_put_data(std::vector<ProfileAdapterTest::TestData> data_set){
  for(auto data : data_set){
    adapter.put_data(data.profile_name, data.status);
  }
}

uint ProfileAdapterTest::entry_count(){
  uint num_entry = 0;
  
  for(auto entry : database->profile_data){
    std::ignore = entry;
    num_entry++;
  }

  return num_entry;
}

void ProfileAdapterTest::check_profile_entry(ProfileTableEntry entry, const std::string &profile_name, const std::string &status)
{
  ASSERT_EQ(entry.profile_name, profile_name);
  ASSERT_EQ(entry.status, status);
}

void ProfileAdapterTest::check_put_data(std::vector<ProfileAdapterTest::TestData> data_set){
  ASSERT_TRUE(database->process_data.empty())  << "We did not add any profile data, so this map should be empty.";
  ASSERT_TRUE(database->log_data.empty())      << "We did not add any log data, so this map should be empty.";
  ASSERT_FALSE(database->profile_data.empty()) << "We added " << data_set.size() << " instances of ProfileEntry, so this map should not be empty.";

  ASSERT_EQ(data_set.size(), entry_count()) << "We expected exactly " << data_set.size() << " entries in the database.";
  ASSERT_EQ(database->profile_data.size(), data_set.size());

  for(auto data : data_set){
    // Attempt to retrieve the data
    auto entry_pair = adapter.get_data(data.profile_name);
    auto entry = entry_pair.first;
    auto did_find_entry = entry_pair.second;

    ASSERT_TRUE(did_find_entry) << "The entry indexed by (" << data.profile_name << ") could not be found";
    check_profile_entry(entry, data.profile_name, data.status);
  }
}

// Test for method put_data
TEST_F(ProfileAdapterTest, PUT_DATA)
{
  TestData data;
  std::vector<TestData> data_set{data};

  try_put_data(data_set);
  check_put_data(data_set);
}

TEST_F(ProfileAdapterTest, PUT_TWO_PROFILES)
{
  TestData data;

  TestData data2;
  data2.profile_name = "second_profile_name";
  
  std::vector<TestData> data_set{data, data2};

  try_put_data(data_set);
  check_put_data(data_set);
}

TEST_F(ProfileAdapterTest, PUT_OVERRIDE_SAME_PROFILE)
{
  TestData data;

  TestData data2;
  data2.status = "complain";

  std::vector<TestData> input_data_set{data, data2};
  std::vector<TestData> output_data_set{data2};

  try_put_data(input_data_set);
  check_put_data(output_data_set);
}
