#ifndef TEST_SRC_TABS_CONTROLLER_STATUS_CONTROLLER_MOCK_TEST_H
#define TEST_SRC_TABS_CONTROLLER_STATUS_CONTROLLER_MOCK_TEST_H

#include "../view/status_mock.h"
#include "../../../../src/tabs/controller/status_controller.cc"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

class StatusControllerChild : public StatusController<StatusMock>
{
  FRIEND_TEST(StatusControllerTest, FILTER_FFF);
  FRIEND_TEST(StatusControllerTest, FILTER_TFF);
  FRIEND_TEST(StatusControllerTest, FILTER_FTF);
  FRIEND_TEST(StatusControllerTest, FILTER_FFT);
  FRIEND_TEST(StatusControllerTest, FILTER_TTF);
  FRIEND_TEST(StatusControllerTest, FILTER_TFT);
  FRIEND_TEST(StatusControllerTest, FILTER_FTT);
  FRIEND_TEST(StatusControllerTest, FILTER_TTT);
  FRIEND_TEST(StatusControllerTest, FILTER_REGEX_TOLOWER);
  FRIEND_TEST(StatusControllerTest, PARSE_JSON_INVALID_FORMAT);
  FRIEND_TEST(StatusControllerTest, PARSE_JSON_NULL_VALUE);
  FRIEND_TEST(StatusControllerTest, PARSE_JSON_STRING_VALUE);
  FRIEND_TEST(StatusControllerTest, PARSE_JSON_INT_VALUE);
  FRIEND_TEST(StatusControllerTest, PARSE_JSON_REAL_VALUE);
  FRIEND_TEST(StatusControllerTest, PARSE_JSON_BOOLEAN_VALUE);
  FRIEND_TEST(StatusControllerTest, PARSE_JSON_ARRAY_VALUE);
  FRIEND_TEST(StatusControllerTest, PARSE_JSON_OBJECT_VALUE);
  FRIEND_TEST(StatusControllerTest, PARSE_JSON_SAMPLE_INPUT);

public:
  explicit StatusControllerChild(std::shared_ptr<StatusMock> tab)
    : StatusController<StatusMock>(tab)
  {  }
};

// Used to avoid linker errors
// For more information, see: https://isocpp.org/wiki/faq/templates#class-templates
template class StatusController<StatusMock>;

#endif