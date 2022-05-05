#ifndef TEST_SRC_TABS_VIEW_LOGS_MOCK_H
#define TEST_SRC_TABS_VIEW_LOGS_MOCK_H

#include "status_mock.h"

// Mock object of the Logs class in ./src/tab/view/logs.h
class LogsMock : public StatusMock
{
  MOCK_METHOD(void, add_data_to_record, (const std::string &unconfined));
};

#endif