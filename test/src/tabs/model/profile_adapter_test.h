#ifndef TEST_SRC_TABS_MODEL_PROFILE_ADAPTER_TEST_H
#define TEST_SRC_TABS_MODEL_PROFILE_ADAPTER_TEST_H

#include "../../../../src/tabs/controller/profiles_controller.cc"
#include "../../../../src/tabs/model/database.h"
#include "../../../../src/tabs/model/profile_adapter.cc"
#include "../../../../src/tabs/view/profiles.h"
#include "database_mock.h"
#include "status_column_record_mock.h"

template class ProfilesController<Profiles, DatabaseMock, ProfileAdapter<DatabaseMock>>;

#endif // TEST_SRC_TABS_MODEL_PROFILE_ADAPTER_TEST_H