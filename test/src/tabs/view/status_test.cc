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
  StatusTest() 
  { }
  
  virtual void SetUp() { }
  void click_everything(Gtk::Widget *obj);
  bool check_label_exists(Gtk::Widget *obj, std::string label_text);
  void set_combobox_text(Gtk::Widget *obj, std::string combo_text);

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

TEST_F(StatusTest, APPLY_SIGNAL_HANDLER)
{
  // We expect that there is exactly one button that can be used to 'apply' changes to a profile.
  EXPECT_CALL(*this, handle_signal()).Times(1);

  auto fun = sigc::mem_fun(*this, &StatusTest::handle_signal);
  sc.set_apply_signal_handler(fun);

  click_everything(&sc);
}

// Recursive method to check whether label exists with `label_text`
bool StatusTest::check_label_exists(Gtk::Widget *obj, std::string label_text)
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

TEST_F(StatusTest, CHECK_APPLY_LABEL_TEXT)
{
  std::string label_text = "arbitrary text for apply label";
  EXPECT_FALSE(check_label_exists(&sc, label_text));
  sc.set_apply_label_text(label_text);
  EXPECT_TRUE(check_label_exists(&sc, label_text));
}

TEST_F(StatusTest, CHECK_STATUS_LABEL_TEXT)
{
  std::string label_text = "arbitrary text for status label";
  EXPECT_FALSE(check_label_exists(&sc, label_text));
  sc.set_apply_label_text(label_text);
  EXPECT_TRUE(check_label_exists(&sc, label_text));
}

// Recursive method to set change the selection text of all comboboxes
void StatusTest::set_combobox_text(Gtk::Widget *obj, std::string combo_text)
{
  // Attempt to cast the object as a Gtk::Button, then click it
  // This will also cast CheckButtons which are a type of Button
  Gtk::ComboBoxText *cb = dynamic_cast<Gtk::ComboBoxText *>(obj);

  // If cb is not a nullptr, then it is a Gtk::ComboBoxText
  if(cb) {
    // click the button
    cb->set_active_text(combo_text);
  }

  // Attempt to cast the object as a Gtk::Container, then enumerate children
  Gtk::Container *parent = dynamic_cast<Gtk::Container *>(obj);

  // If parent is not a nullptr, then it is a Gtk::Container
  if(parent) {
    auto children = parent->get_children();

    for(auto child: children) {
      set_combobox_text(child, combo_text);
    }
  }
}

TEST_F(StatusTest, CHECK_GET_SELECTION_TEXT)
{
  std::string text = "Enforce";
  set_combobox_text(&sc, text);
  EXPECT_EQ(sc.get_selection_text(), text);

  text = "Complain";
  set_combobox_text(&sc, text);
  EXPECT_EQ(sc.get_selection_text(), text);

  text = "Disable";
  set_combobox_text(&sc, text);
  EXPECT_EQ(sc.get_selection_text(), text);
}
