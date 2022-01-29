#include <gmock/gmock.h>
#include <gtest/gtest.h>

class ProfilesMock
{
public:
  // virtual void add_data_to_record(const std::string& data){};
  MOCK_METHOD(void, add_data_to_record, (const std::string& data));
  MOCK_METHOD(void, set_apply_label_text, (const std::string& data));
};
