#ifndef TEST_SRC_TABS_MODEL_LOG_ADAPTER_TEST_H
#define TEST_SRC_TABS_MODEL_LOG_ADAPTER_TEST_H

#include "../../../../src/tabs/model/database.h"
#include "../../../../src/tabs/model/log_adapter.cc"
#include "status_column_record_mock.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

// Should probably move into separate class
class DatabaseChild : public Database
{
private:
    friend class LogAdapterChild;
};

class LogAdapterChild : public LogAdapter<DatabaseChild, StatusColumnRecordMock>
{
public:
  FRIEND_TEST(LogAdapterTest, TEST_FORMAT_LOG_DATA);
  FRIEND_TEST(LogAdapterTest, TEST_FORMAT_TIMESTAMP);

  explicit LogAdapterChild(std::shared_ptr<DatabaseChild> db, std::shared_ptr<StatusColumnRecordMock> cr) 
    : LogAdapter(db, cr)
  { }
};

#endif // TEST_SRC_TABS_MODEL_LOG_ADAPTER_TEST_H