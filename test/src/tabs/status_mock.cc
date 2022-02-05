#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../src/tabs/status.h"

class StatusMock : public Status
{
  FRIEND_TEST(StatusTest, FILTER_FFF);
  FRIEND_TEST(StatusTest, FILTER_TFF);
  FRIEND_TEST(StatusTest, FILTER_FTF);
  FRIEND_TEST(StatusTest, FILTER_FFT);
  FRIEND_TEST(StatusTest, FILTER_TTF);
  FRIEND_TEST(StatusTest, FILTER_TFT);
  FRIEND_TEST(StatusTest, FILTER_FTT);
  FRIEND_TEST(StatusTest, FILTER_TTT);
  FRIEND_TEST(StatusTest, FILTER_REGEX_TOLOWER);
  FRIEND_TEST(StatusTest, VIEW_NOT_NULL);
  FRIEND_TEST(StatusTest, APPLY_SIGNAL_HANDLER);
  FRIEND_TEST(StatusTest, REFRESH_SIGNAL_HANDLER);
  FRIEND_TEST(StatusTest, CHECK_APPLY_LABEL_TEXT);
  FRIEND_TEST(StatusTest, CHECK_STATUS_LABEL_TEXT);
  FRIEND_TEST(StatusTest, CHECK_GET_SELECTION_TEXT);
};
