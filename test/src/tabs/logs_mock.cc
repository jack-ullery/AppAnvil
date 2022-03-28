#include "../../../src/tabs/logs.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

template<class ColumnRecord> class LogsMock : public Logs<ColumnRecord>
{
public:
  /*FRIEND_TEST(LogsTabTest, TEST_ADD_ROW_FROM_LINE);
  FRIEND_TEST(LogsTabTest, TEST_ADD_DATA_TO_RECORD);
  FRIEND_TEST(LogsTabTest, TEST_PARSE_LINE);
  FRIEND_TEST(LogsTabTest, TEST_REFRESH);*/

  explicit LogsMock(std::shared_ptr<ColumnRecord> col_record) : Logs<ColumnRecord>(col_record) { }
};
