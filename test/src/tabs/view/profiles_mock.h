#ifndef TEST_SRC_TABS_VIEW_PROFILES_MOCK_H
#define TEST_SRC_TABS_VIEW_PROFILES_MOCK_H

#include "status_mock.h"

// Mock object of the Profiles class in ./src/tab/view/profiles.h
class ProfilesMock : public StatusMock
{
public:
  // virtual void add_data_to_record(const std::string& data){};
  MOCK_METHOD(void, add_data_to_record, (const std::string &data));
  MOCK_METHOD(void, set_apply_label_text, (const std::string &data));
};

#endif