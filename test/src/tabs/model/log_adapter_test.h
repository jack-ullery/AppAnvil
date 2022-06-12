#ifndef TEST_SRC_TABS_MODEL_LOG_ADAPTER_TEST_H
#define TEST_SRC_TABS_MODEL_LOG_ADAPTER_TEST_H

#include "database_mock.h"
#include "../../../../src/tabs/entries.h"
#include "../../../../src/tabs/model/log_adapter.cc"
#include "status_column_record_mock.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

class LogAdapterChild : public LogAdapter<DatabaseMock, StatusColumnRecordMock>
{
public:
  FRIEND_TEST(LogAdapterTest, TEST_FORMAT_LOG_DATA);
  FRIEND_TEST(LogAdapterTest, TEST_FORMAT_TIMESTAMP);

  explicit LogAdapterChild(std::shared_ptr<DatabaseMock> db, std::shared_ptr<StatusColumnRecordMock> cr) 
    : LogAdapter(db, cr)
  { }
};

#endif // TEST_SRC_TABS_MODEL_LOG_ADAPTER_TEST_H