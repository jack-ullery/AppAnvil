#ifndef TEST_SRC_TABS_MODEL_LOG_ADAPTER_MOCK_TEST_H
#define TEST_SRC_TABS_MODEL_LOG_ADAPTER_MOCK_TEST_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "status_column_record_mock.h"

class LogAdapterMock
{
public:
  MOCK_METHOD(void, put_data, (const time_t &timestamp,
                               const std::string &type, 
                               const std::string &operation,
                               const std::string &profile_name,
                               const unsigned int &pid, 
                               const std::string &status));

  MOCK_METHOD(std::shared_ptr<StatusColumnRecordMock>, get_col_record, ());
};

#endif