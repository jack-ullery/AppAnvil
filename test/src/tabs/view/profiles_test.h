#ifndef TEST_SRC_TABS_CONTROLLER_PROFILES_MOCK_TEST_H
#define TEST_SRC_TABS_CONTROLLER_PROFILES_MOCK_TEST_H

#include "../../../../src/tabs/view/profiles.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <gtkmm/treemodel.h>
#include <gtkmm/treestore.h>

// Test Fixture for Status class
class ProfilesTest : public ::testing::Test
{
public:
  MOCK_METHOD(void, handle_signal, (std::string, std::string, std::string));

protected:
  ProfilesTest() {}

  Glib::RefPtr<Gtk::TreeStore> initialize_store();
  void create_and_select_row(Glib::RefPtr<Gtk::TreeStore> store, std::string profile, std::string status);

  Gtk::TreeModelColumnRecord record;
  Profiles pc;
};

#endif