#ifndef TEST_SRC_TABS_VIEW_PROCESSES_MOCK_H
#define TEST_SRC_TABS_VIEW_PROCESSES_MOCK_H

#include "status_mock.h"

// Mock object of the Processes class in ./src/tab/view/processes.h
class ProcessesMock : public StatusMock
{
public:
  MOCK_METHOD(void, add_data_to_record, (const std::string &confined, const std::string &unconfined));
};

#endif