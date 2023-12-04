#ifndef TEST_SRC_TABS_CONTROLLER_STATUS_CONTROLLER_MOCK_TEST_H
#define TEST_SRC_TABS_CONTROLLER_STATUS_CONTROLLER_MOCK_TEST_H

#include <memory>

#include "../view/status_mock.h"

// Mock object of Status class for use in status and dispatcher_middleman tests
class StatusControllerMock
{
public:
  StatusControllerMock();
  std::shared_ptr<StatusMock> get_tab();

private:
  std::shared_ptr<StatusMock> status_mock;
};

#endif