#include "profiles_test.h"

#include <chrono>
#include <gtest/gtest.h>
#include <gtkmm/comboboxtext.h>
#include <memory>
#include <thread>

using ::testing::_;

TEST_F(ProfilesTest, CHECK_APPLY_LABEL_TEXT)
{
  std::string label_text = "arbitrary text for apply label";

  bool label_exists = (pc.p_apply_info_text->get_text().compare(label_text) == 0);
  EXPECT_FALSE(label_exists);

  pc.set_apply_label_text(label_text);

  label_exists = (pc.p_apply_info_text->get_text().compare(label_text) == 0);
  EXPECT_TRUE(label_exists);
}

TEST_F(ProfilesTest, APPLY_NO_ROWS_SELECTED)
{

  EXPECT_CALL(*this, handle_signal(_, _, _)).Times(0);

  auto fun = sigc::mem_fun(*this, &ProfilesTest::handle_signal);

  pc.set_status_change_signal_handler(fun);
  pc.p_apply_button->activate();
}
