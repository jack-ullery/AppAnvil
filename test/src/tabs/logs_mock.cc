#include "../../../src/tabs/view/logs.cc"
#include "status_column_record_mock.cc"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

template<class ColumnRecord> class LogsMock : public Logs<ColumnRecord>
{
public:
  FRIEND_TEST(LogsTest, TEST_FORMAT_LOG_DATA);
  FRIEND_TEST(LogsTest, TEST_FORMAT_TIMESTAMP);
  FRIEND_TEST(LogsTest, TEST_ADD_ROW_FROM_JSON);
  FRIEND_TEST(LogsTest, TEST_ADD_DATA_TO_RECORD_VALID);
  FRIEND_TEST(LogsTest, TEST_ADD_DATA_TO_RECORD_INVALID);
  FRIEND_TEST(LogsTest, TEST_REFRESH);

  explicit LogsMock(std::shared_ptr<ColumnRecord> col_record) : Logs<ColumnRecord>(col_record) { }
};

// Used to avoid linker errors
// For more information, see: https://isocpp.org/wiki/faq/templates#class-templates
template class Logs<StatusColumnRecordMock>;