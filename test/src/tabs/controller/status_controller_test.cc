#include "status_controller_test.h"

#include <chrono>
#include <gtest/gtest.h>
#include <gtkmm/comboboxtext.h>
#include <memory>
#include <thread>

// Test Fixture for Status class
class StatusControllerTest : public ::testing::Test
{
public:
  MOCK_METHOD(void, handle_signal, ());

protected:
  StatusControllerTest()
    : sm{ new StatusMock() },
      sc(sm)
  {
  }

  virtual void SetUp() {}
  void click_everything(Gtk::Widget *obj);
  bool check_label_exists(Gtk::Widget *obj, std::string label_text);
  void set_combobox_text(Gtk::Widget *obj, std::string combo_text);

  std::shared_ptr<StatusMock> sm;
  StatusControllerChild sc;
};

TEST_F(StatusControllerTest, FILTER_FFF)
{
  bool res = sc.should_filter("abcdEFGH", "cDEf", false, false, false);
  EXPECT_TRUE(res);

  res = sc.should_filter("abcdEFGH", "gHj", false, false, false);
  EXPECT_FALSE(res);
}

TEST_F(StatusControllerTest, FILTER_TFF)
{
  bool res = sc.should_filter("abcdEFGH", "c*f", true, false, false);
  EXPECT_TRUE(res);

  res = sc.should_filter("abcdEFGH", "g*j", true, false, false);
  EXPECT_FALSE(res);
}

TEST_F(StatusControllerTest, FILTER_FTF)
{
  bool res = sc.should_filter("abcdEFGH", "cdEF", false, true, false);
  EXPECT_TRUE(res);

  res = sc.should_filter("abcdEFGH", "cdef", false, true, false);
  EXPECT_FALSE(res);
}

TEST_F(StatusControllerTest, FILTER_FFT)
{
  bool res = sc.should_filter("abcdEFGH", "abcdEFGh", false, false, true);
  EXPECT_TRUE(res);

  res = sc.should_filter("abcdEFGH", "abcdEFG", false, false, true);
  EXPECT_FALSE(res);
}

TEST_F(StatusControllerTest, FILTER_TTF)
{
  bool res = sc.should_filter("abcdEFGH", "c.*F", true, true, false);
  EXPECT_TRUE(res);

  res = sc.should_filter("abcdEFGH", "c.*f", true, true, false);
  EXPECT_FALSE(res);
}

TEST_F(StatusControllerTest, FILTER_TFT)
{
  bool res = sc.should_filter("abcdEFGH", "[a-z]{8}", true, false, true);
  EXPECT_TRUE(res);

  res = sc.should_filter("abcdEFGH", "[a-z]{7}", true, false, true);
  EXPECT_FALSE(res);
}

TEST_F(StatusControllerTest, FILTER_FTT)
{
  bool res = sc.should_filter("abcdEFGH", "abcdEFGH", false, true, true);
  EXPECT_TRUE(res);

  res = sc.should_filter("abcdEFGH", "ABCDEFGH", false, true, true);
  EXPECT_FALSE(res);
}

TEST_F(StatusControllerTest, FILTER_TTT)
{
  bool res = sc.should_filter("abcdEFGH", "\\w{8}", true, true, true);
  EXPECT_TRUE(res);

  res = sc.should_filter("abcdEFGH", "\\w{7}", true, true, true);
  EXPECT_FALSE(res);
}

TEST_F(StatusControllerTest, FILTER_REGEX_TOLOWER)
{
  bool res = sc.should_filter("abcdefgh ", "\\S{8}", true, true, false);
  EXPECT_TRUE(res);

  res = sc.should_filter("abcdefgh ", "\\S{8}", true, false, false);
  EXPECT_TRUE(res);
}

TEST_F(StatusControllerTest, GET_TAB)
{
  // Check the get_tab() method actually returns the correct mocked tab
  auto tab = sc.get_tab();
  EXPECT_EQ(tab, sm) << "get_tab() does not return the expected value";
}

// Recursive method to click all the checkboxes that are descendents of a Container Widget
void StatusControllerTest::click_everything(Gtk::Widget *obj)
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

// Recursive method to check whether label exists with `label_text`
bool StatusControllerTest::check_label_exists(Gtk::Widget *obj, std::string label_text)
{
  // Attempt to cast the object as a Gtk::Label
  Gtk::Label *lab = dynamic_cast<Gtk::Label *>(obj);

  // If cb is not a nullptr, then it is a Gtk::Label
  if (lab) {
    // click the button
    return (lab->get_text().compare(label_text) == 0);
  }

  // Attempt to cast the object as a Gtk::Container, then enumerate children
  Gtk::Container *parent = dynamic_cast<Gtk::Container *>(obj);

  // If parent is not a nullptr, then it is a Gtk::Container
  if (parent) {
    auto children = parent->get_children();

    for (auto child : children) {
      bool re = check_label_exists(child, label_text);
      // If there was a child Gtk::Label with label_text as its text
      if (re) {
        return true;
      }
    }
  }

  return false;
}

// Recursive method to set change the selection text of all comboboxes
void StatusControllerTest::set_combobox_text(Gtk::Widget *obj, std::string combo_text)
{
  // Attempt to cast the object as a Gtk::Button, then click it
  // This will also cast CheckButtons which are a type of Button
  Gtk::ComboBoxText *cb = dynamic_cast<Gtk::ComboBoxText *>(obj);

  // If cb is not a nullptr, then it is a Gtk::ComboBoxText
  if (cb) {
    // click the button
    cb->set_active_text(combo_text);
  }

  // Attempt to cast the object as a Gtk::Container, then enumerate children
  Gtk::Container *parent = dynamic_cast<Gtk::Container *>(obj);

  // If parent is not a nullptr, then it is a Gtk::Container
  if (parent) {
    auto children = parent->get_children();

    for (auto child : children) {
      set_combobox_text(child, combo_text);
    }
  }
}

// Test for method parse_JSON(...) with an argument that is not in valid JSON format
// Should return a best effort guess of the JSON, without throwing any exception
TEST_F(StatusControllerTest, PARSE_JSON_INVALID_FORMAT)
{
  std::string raw_json = "";
  EXPECT_NO_THROW(sc.parse_JSON(raw_json));
}

// Test for method parse_JSON(...) with an argument containing a null value
TEST_F(StatusControllerTest, PARSE_JSON_NULL_VALUE)
{
  std::string raw_json = "{\"testKey\": null}";
  Json::Value res      = sc.parse_JSON(raw_json);
  ASSERT_FALSE(res.isNull()) << "result of parsing json should not be null";
  EXPECT_EQ(res.type(), Json::ValueType::objectValue) << "ValueType of result should be ValueType.objectValue";
  EXPECT_EQ(res["testKey"].type(), Json::ValueType::nullValue) << "ValueType of property testKey should be ValueType.nullValue";
  EXPECT_TRUE(res["testKey"].isNull()) << "value of property testKey should be null";
}

// Test for method parse_JSON(...) with an argument containing a string value
TEST_F(StatusControllerTest, PARSE_JSON_STRING_VALUE)
{
  std::string raw_json = "{\"testKey\": \"testString\"}";
  Json::Value res      = sc.parse_JSON(raw_json);
  ASSERT_FALSE(res.isNull()) << "result of parsing json should not be null";
  EXPECT_EQ(res.type(), Json::ValueType::objectValue) << "ValueType of result should be ValueType.objectValue";
  EXPECT_EQ(res["testKey"].type(), Json::ValueType::stringValue) << "ValueType of property testKey should be ValueType.stringValue";
  EXPECT_EQ(res["testKey"], "testString") << "value of property testKey should be testString";
}

// Test for method parse_JSON(...) with an argument containing an int value
TEST_F(StatusControllerTest, PARSE_JSON_INT_VALUE)
{
  std::string raw_json = "{\"testKey\": 10}";
  Json::Value res      = sc.parse_JSON(raw_json);
  ASSERT_FALSE(res.isNull()) << "result of parsing json should not be null";
  EXPECT_EQ(res.type(), Json::ValueType::objectValue) << "ValueType of result should be ValueType.objectValue";
  EXPECT_EQ(res["testKey"].type(), Json::ValueType::intValue) << "ValueType of property testKey should be ValueType.intValue";
  EXPECT_EQ(res["testKey"], 10) << "value of property testKey should be 10";
}

// Test for method parse_JSON(...) with an argument containing a real (double) value
TEST_F(StatusControllerTest, PARSE_JSON_REAL_VALUE)
{
  std::string raw_json = "{\"testKey\": 5.0}";
  Json::Value res      = sc.parse_JSON(raw_json);
  ASSERT_FALSE(res.isNull()) << "result of parsing json should not be null";
  EXPECT_EQ(res.type(), Json::ValueType::objectValue) << "ValueType of result should be ValueType.objectValue";
  EXPECT_EQ(res["testKey"].type(), Json::ValueType::realValue) << "ValueType of property testKey should be ValueType.realValue";
  EXPECT_EQ(res["testKey"], 5.0) << "value of property testKey should be 5.0";
}

// Test for method parse_JSON(...) with an argument containing a boolean value
TEST_F(StatusControllerTest, PARSE_JSON_BOOLEAN_VALUE)
{
  std::string raw_json = "{\"testKey\": true}";
  Json::Value res      = sc.parse_JSON(raw_json);
  ASSERT_FALSE(res.isNull()) << "result of parsing json should not be null";
  EXPECT_EQ(res.type(), Json::ValueType::objectValue) << "ValueType of result should be ValueType.objectValue";
  EXPECT_EQ(res["testKey"].type(), Json::ValueType::booleanValue) << "ValueType of property testKey should be ValueType.booleanValue";
  EXPECT_EQ(res["testKey"], true) << "value of property testKey should be true";
}

// Test for method parse_JSON(...) with an argument containing an array value
TEST_F(StatusControllerTest, PARSE_JSON_ARRAY_VALUE)
{
  std::string raw_json = "{\"testKey\": [\"string1\", \"string2\", \"string3\"]}";
  Json::Value res      = sc.parse_JSON(raw_json);
  ASSERT_FALSE(res.isNull()) << "result of parsing json should not be null";
  EXPECT_EQ(res.type(), Json::ValueType::objectValue) << "ValueType of result should be ValueType.objectValue";
  EXPECT_EQ(res["testKey"].type(), Json::ValueType::arrayValue) << "ValueType of property testKey should be ValueType.arrayValue";
  EXPECT_EQ(res["testKey"][0], "string1") << "value of first element of property testKey should be string1";
  EXPECT_EQ(res["testKey"][1], "string2") << "value of second element of property testKey should be string2";
  EXPECT_EQ(res["testKey"][2], "string3") << "value of third element of property testKey should be string3";
}

// Test for method parse_JSON(...) with an argument containing an object value
TEST_F(StatusControllerTest, PARSE_JSON_OBJECT_VALUE)
{
  std::string raw_json = "{\"testKey\": {\"subKey1\": \"string1\", \"subKey2\": 20, \"subKey3\": true}}";
  Json::Value res      = sc.parse_JSON(raw_json);
  ASSERT_FALSE(res.isNull()) << "result of parsing json should not be null";
  EXPECT_EQ(res.type(), Json::ValueType::objectValue) << "ValueType of result should be ValueType.objectValue";
  EXPECT_EQ(res["testKey"].type(), Json::ValueType::objectValue) << "ValueType of property testKey should be ValueType.objectValue";
  EXPECT_EQ(res["testKey"]["subKey1"], "string1") << "value of property subKey1 within testKey should be string1";
  EXPECT_EQ(res["testKey"]["subKey2"], 20) << "value of property subKey2 within testKey should be 20";
  EXPECT_EQ(res["testKey"]["subKey3"], true) << "value of property subKey3 within testKey should be true";
}

// Test for method parse_JSON(...) with an argument containing a sample snippet of output taken from running
// 'aa-status --json' in a linux terminal. The passed argument is an example of the types of arguments that the
// method parse_JSON(...) will receive during actual use
TEST_F(StatusControllerTest, PARSE_JSON_SAMPLE_INPUT)
{
  std::string raw_json =
    "{\"version\" : \"1\", \"profiles\" : {\"/snap/snapd/14978/usr/lib/snapd/snap-confine\" : \"enforce\", \"/usr/bin/evince\" : "
    "\"complain\"}, \"processes\": {\"/usr/sbin/cupsd\" : [ {\"profile\" : \"/usr/sbin/cupsd\", \"pid\" : \"616\", \"status\" : "
    "\"enforce\"} ], \"/usr/sbin/cups-browsed\" : [ {\"profile\" : \"/usr/sbin/NetworkManager\", \"pid\" : \"619\", \"status\" : "
    "\"unconfined\"} ] }}";
  Json::Value res = sc.parse_JSON(raw_json);
  ASSERT_FALSE(res.isNull()) << "result of parsing json should not be null";
  EXPECT_EQ(res.type(), Json::ValueType::objectValue) << "ValueType of result should be ValueType.objectValue";
  EXPECT_EQ(res["profiles"].type(), Json::ValueType::objectValue) << "ValueType of property profiles should be ValueType.objectValue";
  EXPECT_EQ(res["profiles"]["/snap/snapd/14978/usr/lib/snapd/snap-confine"].type(), Json::ValueType::stringValue)
    << "ValueType of property /snap/snapd/14978/usr/lib/snapd/snap-confine within profiles should be ValueType.stringValue";
  EXPECT_EQ(res["profiles"]["/snap/snapd/14978/usr/lib/snapd/snap-confine"], "enforce")
    << "value of property /snap/snapd/14978/usr/lib/snapd/snap-confine should be enforce";
  EXPECT_EQ(res["profiles"]["/usr/bin/evince"].type(), Json::ValueType::stringValue)
    << "ValueType of property /usr/bin/evince within profiles should be ValueType.stringValue";
  EXPECT_EQ(res["profiles"]["/usr/bin/evince"], "complain") << "value of property /usr/bin/evince within profiles should be complain";
  EXPECT_EQ(res["processes"]["/usr/sbin/cupsd"].type(), Json::ValueType::arrayValue)
    << "ValueType of property /usr/sbin/cupsd within processes should be ValueType.arrayValue";
  EXPECT_EQ(res["processes"]["/usr/sbin/cupsd"][0]["pid"], "616")
    << "value of property pid within /usr/sbin/cupsd in processes should be 616";
  EXPECT_EQ(res["processes"]["/usr/sbin/cupsd"][0]["status"], "enforce")
    << "value of property status within /usr/sbin/cupsd in processes should be enforce";
  EXPECT_EQ(res["processes"]["/usr/sbin/cups-browsed"].type(), Json::ValueType::arrayValue)
    << "ValueType of property /usr/sbin/cups-browsed within processes should be ValueType.arrayValue";
  EXPECT_EQ(res["processes"]["/usr/sbin/cups-browsed"][0]["pid"], "619")
    << "value of property pid within /usr/sbin/cups-browsed in processes should be 619";
  EXPECT_EQ(res["processes"]["/usr/sbin/cups-browsed"][0]["status"], "unconfined")
    << "value of property status within /usr/sbin/cups-browsed in processes should be unconfined";
}
