#include <gmock/gmock.h>
#include <gtest/gtest.h>

class LogsMock
{
public:
  MOCK_METHOD(void, add_data_to_record, (const std::string &data));
};
