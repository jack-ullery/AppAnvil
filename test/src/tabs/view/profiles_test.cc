#include "profiles_test.h"

#include <chrono>
#include <gtest/gtest.h>
#include <gtkmm/comboboxtext.h>
#include <memory>
#include <thread>

// Test Fixture for Status class
class ProfilesTest : public ::testing::Test
{
public:
  MOCK_METHOD(void, handle_signal, ());

protected:
  ProfilesTest() 
  { }

  virtual void SetUp() { }
  void click_everything(Gtk::Widget *obj);
  bool check_label_exists(Gtk::Widget *obj, std::string label_text);

  ProfilesChild pc;
};

// Recursive method to click all the checkboxes that are descendents of a Container Widget
void ProfilesTest::click_everything(Gtk::Widget *obj)
{
  // Attempt to cast the object as a Gtk::Button, then click it
  // This will also cast CheckButtons which are a type of Button
  Gtk::Button *bu = dynamic_cast<Gtk::Button *>(obj);

  // If bu is not a nullptr, then it is a Gtk::Button
  if(bu) {
    // click the button
    bu->clicked();
  }

  // Attempt to cast the object as a Gtk::Container, then enumerate children
  Gtk::Container *parent = dynamic_cast<Gtk::Container *>(obj);

  // If parent is not a nullptr, then it is a Gtk::Container
  if(parent) {
    auto children = parent->get_children();

    for(auto child: children) {
      click_everything(child);
    }
  }
}

// Recursive method to check whether label exists with `label_text`
bool ProfilesTest::check_label_exists(Gtk::Widget *obj, std::string label_text)
{
  // Attempt to cast the object as a Gtk::Label
  Gtk::Label *lab = dynamic_cast<Gtk::Label *>(obj);

  // If cb is not a nullptr, then it is a Gtk::Label
  if(lab) {
    // click the button
    return (lab->get_text().compare(label_text) == 0);
  }

  // Attempt to cast the object as a Gtk::Container, then enumerate children
  Gtk::Container *parent = dynamic_cast<Gtk::Container *>(obj);

  // If parent is not a nullptr, then it is a Gtk::Container
  if(parent) {
    auto children = parent->get_children();

    for(auto child: children) {
      bool re = check_label_exists(child, label_text);
      // If there was a child Gtk::Label with label_text as its text
      if(re) {
        return true;
      }
    }
  }

  return false;
}

TEST_F(ProfilesTest, CHECK_APPLY_LABEL_TEXT)
{
  std::string label_text = "arbitrary text for apply label";
  EXPECT_FALSE(check_label_exists(&pc, label_text));
  pc.set_apply_label_text(label_text);
  EXPECT_TRUE(check_label_exists(&pc, label_text));
}

TEST_F(ProfilesTest, APPLY_SIGNAL_HANDLER)
{
  // We know there are exactly three CheckButtons and one SearchEntry that can be modified to send a signal
  // However, I was getting some problems emitting the `signal_search_changed` event, so this does not test the SearchEntry
  // If there are a different amount of CheckButtons, this will fail.
  EXPECT_CALL(*this, handle_signal()).Times(3);

  auto fun = sigc::mem_fun(*this, &ProfilesTest::handle_signal);
  pc.set_status_change_signal_handler(fun);

  click_everything(&pc);
}
