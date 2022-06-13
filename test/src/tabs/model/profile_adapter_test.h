#ifndef TEST_SRC_TABS_MODEL_PROFILE_ADAPTER_TEST_H
#define TEST_SRC_TABS_MODEL_PROFILE_ADAPTER_TEST_H

#include "../../../../src/tabs/controller/profiles_controller.cc"
#include "../../../../src/tabs/model/database.h"
#include "../../../../src/tabs/model/profile_adapter.cc"
#include "../../../../src/tabs/view/profiles.h"
#include "database_mock.h"
#include "status_column_record_mock.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

class ProfileAdapterChild : public ProfileAdapter<Database>
{
public:
  FRIEND_TEST(ProfileAdapterTest, TEST_PUT_DATA);

  explicit ProfileAdapterChild(std::shared_ptr<Database> db, std::shared_ptr<StatusColumnRecord> cr) 
    : ProfileAdapter(db, cr)
  { }
};

template class ProfilesController<Profiles, DatabaseMock, ProfileAdapter<DatabaseMock>>;

#endif // TEST_SRC_TABS_MODEL_PROFILE_ADAPTER_TEST_H