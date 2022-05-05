#ifndef TEST_SRC_TABS_CONTROLLER_STATUS_CONTROLLER_MOCK_TEST_H
#define TEST_SRC_TABS_CONTROLLER_STATUS_CONTROLLER_MOCK_TEST_H

#include "../../../../src/tabs/view/status.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

class StatusChild : public Status
{
  FRIEND_TEST(StatusTest, VIEW_NOT_NULL);
  FRIEND_TEST(StatusTest, APPLY_SIGNAL_HANDLER);
  FRIEND_TEST(StatusTest, REFRESH_SIGNAL_HANDLER);
  FRIEND_TEST(StatusTest, CHECK_APPLY_LABEL_TEXT);
  FRIEND_TEST(StatusTest, CHECK_STATUS_LABEL_TEXT);
  FRIEND_TEST(StatusTest, CHECK_GET_SELECTION_TEXT);
};

#endif