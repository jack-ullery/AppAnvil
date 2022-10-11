#include "status_test.h"

#include <chrono>
#include <gtest/gtest.h>
#include <gtkmm/comboboxtext.h>
#include <memory>
#include <thread>

// Test Fixture for Status class
class StatusTest : public ::testing::Test
{
public:
  MOCK_METHOD(void, handle_signal, ());

protected:
  StatusTest() {}

  virtual void SetUp() {}
  void click_everything(Gtk::Widget *obj);

  StatusChild sc;
};

TEST_F(StatusTest, VIEW_NOT_NULL)
{
  // We know `nullptr` evaluates to `0` which is `FALSE`
  // This check `sc->get_view` does not return a null pointer
  EXPECT_TRUE(sc.get_view()) << "get_view() unexpectedly returned a nullptr.";
}

// Recursive method to click all the checkboxes that are descendents of a Container Widget
void StatusTest::click_everything(Gtk::Widget *obj)
{
  // Attempt to cast the object as a Gtk::Button, then click it
  // This will also cast CheckButtons which are a type of Button
  Gtk::Button *bu = dynamic_cast<Gtk::Button *>(obj);

  // If bu is not a nullptr, then it is a Gtk::Button
  if (bu) {
    // click the button
    bu->clicked();
  }

  // Attempt to cast the object as a Gtk::Container, then enumerate children
  Gtk::Container *parent = dynamic_cast<Gtk::Container *>(obj);

  // If parent is not a nullptr, then it is a Gtk::Container
  if (parent) {
    auto children = parent->get_children();

    for (auto child : children) {
      click_everything(child);
    }
  }
}

TEST_F(StatusTest, REFRESH_SIGNAL_HANDLER)
{
  // We know there are exactly three CheckButtons and one SearchEntry that can be modified to send a signal
  // However, I was getting some problems emitting the `signal_search_changed` event, so this does not test the SearchEntry
  // If there are a different amount of CheckButtons, this will fail.
  EXPECT_CALL(*this, handle_signal()).Times(3);

  auto fun = sigc::mem_fun(*this, &StatusTest::handle_signal);
  sc.set_refresh_signal_handler(fun);

  click_everything(&sc);
}
