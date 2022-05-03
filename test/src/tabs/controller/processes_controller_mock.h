#ifndef TEST_SRC_TABS_CONTROLLER_PROCESSES_CONTROLLER_MOCK_TEST_H
#define TEST_SRC_TABS_CONTROLLER_PROCESSES_CONTROLLER_MOCK_TEST_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>

// Mock object of Processes class for use in status and dispatcher_middleman tests
class ProcessesStatusMock
{
public:
  MOCK_METHOD(void, add_data_to_record, (const std::string &unconfined));
};

#endif