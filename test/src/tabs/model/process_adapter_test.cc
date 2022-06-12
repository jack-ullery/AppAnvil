#include "jsoncpp/json/json.h"
#include "process_adapter_test.h"
#include "../model/status_column_record_mock.h"

#include <gmock/gmock-matchers.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <regex>
#include <string>

// Test Fixture for Logs class
class ProcessAdapterTest : public ::testing::Test
{
protected:
  ProcessAdapterTest() 
  : col_record_mock{new StatusColumnRecordMock()},
    database{new DatabaseMock()},
    proc_adapter(database, col_record_mock)
  { }

  // Mock objects
  std::shared_ptr<StatusColumnRecordMock> col_record_mock;
  std::shared_ptr<DatabaseMock> database;

  ProcessAdapterChild proc_adapter;
};

// Test for method put_data
TEST_F(ProcessAdapterTest, TEST_PUT_DATA)
{

  ASSERT_TRUE(false) << "This test is not completed yet!!!";
}
