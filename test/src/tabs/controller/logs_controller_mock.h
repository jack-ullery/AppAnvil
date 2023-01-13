#ifndef TEST_SRC_TABS_CONTROLLER_LOGS_CONTROLLER_MOCK_TEST_H
#define TEST_SRC_TABS_CONTROLLER_LOGS_CONTROLLER_MOCK_TEST_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <list>

#include "../../../../src/threads/log_record.h"

// Mock object of LogsController class for use in status and dispatcher_middleman tests
class LogsControllerMock
{
public:
  MOCK_METHOD(void, add_data_to_record, (const std::list<std::shared_ptr<LogRecord>> &data));
  MOCK_METHOD(unsigned int, num_visible_rows, ());
  MOCK_METHOD(void, refresh, ());
};

#endif