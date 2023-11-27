#ifndef TEST_SRC_TABS_CONTROLLER_PROCESSES_CONTROLLER_MOCK_TEST_H
#define TEST_SRC_TABS_CONTROLLER_PROCESSES_CONTROLLER_MOCK_TEST_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "status_controller_mock.h"

// Mock object of Processes class for use in status and dispatcher_middleman tests
class ProcessesControllerMock : public StatusControllerMock
{
public:
  MOCK_METHOD(void, add_data_to_record, (const std::string &unconfined));
};

#endif