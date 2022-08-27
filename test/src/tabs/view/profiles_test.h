#ifndef TEST_SRC_TABS_CONTROLLER_PROFILES_MOCK_TEST_H
#define TEST_SRC_TABS_CONTROLLER_PROFILES_MOCK_TEST_H

#include "../../../../src/tabs/view/profiles.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

// Test Fixture for Status class
class ProfilesTest : public ::testing::Test
{
public:
  MOCK_METHOD(void, handle_signal, (std::string, std::string, std::string));

protected:
  ProfilesTest() {}

  virtual void SetUp() {}
  void click_everything(Gtk::Widget *obj);
  bool check_label_exists(Gtk::Widget *obj, std::string label_text);

  Profiles pc;
};

#endif