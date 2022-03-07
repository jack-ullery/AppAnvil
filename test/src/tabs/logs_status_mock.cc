#include <gmock/gmock.h>
#include <gtest/gtest.h>

// Mock object of Logs class for use in status and dispatcher_middleman tests
class LogsStatusMock
{
public:
  MOCK_METHOD(void, add_data_to_record, (const std::string &data));
};
