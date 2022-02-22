#include <gmock/gmock.h>
#include <gtest/gtest.h>

class ProcessesStatusMock
{
public:
  MOCK_METHOD(void, add_data_to_record, (const std::string &confined, const std::string &unconfined));
};
