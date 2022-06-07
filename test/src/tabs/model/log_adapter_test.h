#ifndef TEST_SRC_TABS_MODEL_LOG_ADAPTER_TEST_H
#define TEST_SRC_TABS_MODEL_LOG_ADAPTER_TEST_H

#include "../model/database_mock.h"
#include "../../../../src/tabs/model/log_adapter.cc"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

class LogAdapterChild : public LogAdapter<DatabaseMock>
{
public:
  FRIEND_TEST(LogAdapterTest, TEST_FORMAT_LOG_DATA);
  FRIEND_TEST(LogAdapterTest, TEST_FORMAT_TIMESTAMP);

  explicit LogAdapterChild(std::shared_ptr<DatabaseMock> db) 
    : LogAdapter(db)
  { }
};

#endif // TEST_SRC_TABS_MODEL_LOG_ADAPTER_TEST_H