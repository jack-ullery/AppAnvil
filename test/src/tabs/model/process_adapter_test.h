#ifndef TEST_SRC_TABS_MODEL_PROCESS_ADAPTER_TEST_H
#define TEST_SRC_TABS_MODEL_PROCESS_ADAPTER_TEST_H

#include "../../../../src/tabs/model/database.h"
#include "../../../../src/tabs/model/process_adapter.cc"
#include "database_mock.h"
#include "status_column_record_mock.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

class ProcessAdapterChild : public ProcessAdapter<DatabaseMock, StatusColumnRecordMock>
{
public:
  FRIEND_TEST(ProcessAdapterTest, TEST_FORMAT_LOG_DATA);
  FRIEND_TEST(ProcessAdapterTest, TEST_FORMAT_TIMESTAMP);

  explicit ProcessAdapterChild(std::shared_ptr<DatabaseMock> db, std::shared_ptr<StatusColumnRecordMock> cr) 
    : ProcessAdapter(db, cr)
  { }
};

#endif // TEST_SRC_TABS_MODEL_PROCESS_ADAPTER_TEST_H