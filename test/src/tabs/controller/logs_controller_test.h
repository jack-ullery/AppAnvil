#ifndef TEST_SRC_TABS_CONTROLLER_LOGS_CONTROLLER_TEST_H
#define TEST_SRC_TABS_CONTROLLER_LOGS_CONTROLLER_TEST_H

#include "../../../../src/tabs/controller/logs_controller.cc"
#include "../../../../src/tabs/controller/status_controller.cc"
#include "../model/log_adapter_mock.h"
#include "../view/logs_mock.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

class LogsControllerChild : public LogsController<LogsMock, Database, LogAdapterMock>
{
public:
  FRIEND_TEST(LogsTest, TEST_FORMAT_LOG_DATA);
  FRIEND_TEST(LogsTest, TEST_FORMAT_TIMESTAMP);
  FRIEND_TEST(LogsTest, TEST_ADD_ROW_FROM_JSON);
  FRIEND_TEST(LogsTest, TEST_ADD_DATA_TO_RECORD_VALID);
  FRIEND_TEST(LogsTest, TEST_ADD_DATA_TO_RECORD_INVALID);
  FRIEND_TEST(LogsTest, TEST_REFRESH);

  explicit LogsControllerChild(std::shared_ptr<LogAdapterMock> adapter, std::shared_ptr<LogsMock> logs) 
    : LogsController(adapter, logs) 
  { }
};

// Used to avoid linker errors
// For more information, see: https://isocpp.org/wiki/faq/templates#class-templates
template class StatusController<LogsMock>;

#endif // TEST_SRC_TABS_CONTROLLER_LOGS_CONTROLLER_TEST_H