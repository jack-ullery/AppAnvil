// #include "status_mock.cc"

// #include <chrono>
// #include <gtest/gtest.h>
// #include <gtkmm/comboboxtext.h>
// #include <thread>

// // Test Fixture for Status class
// class StatusTest : public ::testing::Test
// {
// public:
//   MOCK_METHOD(void, handle_signal, ());

// protected:
//   StatusTest() { }
//   virtual void SetUp() { }
//   void click_everything(Gtk::Widget *obj);
//   bool check_label_exists(Gtk::Widget *obj, std::string label_text);
//   void set_combobox_text(Gtk::Widget *obj, std::string combo_text);

//   StatusMock sm;
// };

// TEST_F(StatusTest, FILTER_FFF)
// {
//   bool res = StatusMock::filter("abcdEFGH", "cDEf", false, false, false);
//   EXPECT_TRUE(res);

//   res = StatusMock::filter("abcdEFGH", "gHj", false, false, false);
//   EXPECT_FALSE(res);
// }

// TEST_F(StatusTest, FILTER_TFF)
// {
//   bool res = StatusMock::filter("abcdEFGH", "c*f", true, false, false);
//   EXPECT_TRUE(res);

//   res = StatusMock::filter("abcdEFGH", "g*j", true, false, false);
//   EXPECT_FALSE(res);
// }

// TEST_F(StatusTest, FILTER_FTF)
// {
//   bool res = StatusMock::filter("abcdEFGH", "cdEF", false, true, false);
//   EXPECT_TRUE(res);

//   res = StatusMock::filter("abcdEFGH", "cdef", false, true, false);
//   EXPECT_FALSE(res);
// }

// TEST_F(StatusTest, FILTER_FFT)
// {
//   bool res = StatusMock::filter("abcdEFGH", "abcdEFGh", false, false, true);
//   EXPECT_TRUE(res);

//   res = StatusMock::filter("abcdEFGH", "abcdEFG", false, false, true);
//   EXPECT_FALSE(res);
// }

// TEST_F(StatusTest, FILTER_TTF)
// {
//   bool res = StatusMock::filter("abcdEFGH", "c.*F", true, true, false);
//   EXPECT_TRUE(res);

//   res = StatusMock::filter("abcdEFGH", "c.*f", true, true, false);
//   EXPECT_FALSE(res);
// }

// TEST_F(StatusTest, FILTER_TFT)
// {
//   bool res = StatusMock::filter("abcdEFGH", "[a-z]{8}", true, false, true);
//   EXPECT_TRUE(res);

//   res = StatusMock::filter("abcdEFGH", "[a-z]{7}", true, false, true);
//   EXPECT_FALSE(res);
// }

// TEST_F(StatusTest, FILTER_FTT)
// {
//   bool res = StatusMock::filter("abcdEFGH", "abcdEFGH", false, true, true);
//   EXPECT_TRUE(res);

//   res = StatusMock::filter("abcdEFGH", "ABCDEFGH", false, true, true);
//   EXPECT_FALSE(res);
// }

// TEST_F(StatusTest, FILTER_TTT)
// {
//   bool res = StatusMock::filter("abcdEFGH", "\\w{8}", true, true, true);
//   EXPECT_TRUE(res);

//   res = StatusMock::filter("abcdEFGH", "\\w{7}", true, true, true);
//   EXPECT_FALSE(res);
// }

// TEST_F(StatusTest, FILTER_REGEX_TOLOWER)
// {
//   bool res = StatusMock::filter("abcdefgh ", "\\S{8}", true, true, false);
//   EXPECT_TRUE(res);

//   res = StatusMock::filter("abcdefgh ", "\\S{8}", true, false, false);
//   EXPECT_TRUE(res);
// }

// TEST_F(StatusTest, VIEW_NOT_NULL)
// {
//   // We know `nullptr` evaluates to `0` which is `FALSE`
//   // This check `sm->get_view` does not return a null pointer
//   EXPECT_TRUE(sm.get_view()) << "get_view() unexpectedly returned a nullptr.";
// }

// // Recursive method to click all the checkboxes that are descendents of a Container Widget
// void StatusTest::click_everything(Gtk::Widget *obj)
// {
//   // Attempt to cast the object as a Gtk::Button, then click it
//   // This will also cast CheckButtons which are a type of Button
//   Gtk::Button *bu = dynamic_cast<Gtk::Button *>(obj);

//   // If bu is not a nullptr, then it is a Gtk::Button
//   if(bu) {
//     // click the button
//     bu->clicked();
//   }

//   // Attempt to cast the object as a Gtk::Container, then enumerate children
//   Gtk::Container *parent = dynamic_cast<Gtk::Container *>(obj);

//   // If parent is not a nullptr, then it is a Gtk::Container
//   if(parent) {
//     auto children = parent->get_children();

//     for(auto child: children) {
//       click_everything(child);
//     }
//   }
// }

// TEST_F(StatusTest, REFRESH_SIGNAL_HANDLER)
// {
//   // We know there are exactly three CheckButtons and one SearchEntry that can be modified to send a signal
//   // However, I was getting some problems emitting the `signal_search_changed` event, so this does not test the SearchEntry
//   // If there are a different amount of CheckButtons, this will fail.
//   EXPECT_CALL(*this, handle_signal()).Times(3);

//   auto fun = sigc::mem_fun(*this, &StatusTest::handle_signal);
//   sm.set_refresh_signal_handler(fun);

//   click_everything(&sm);
// }

// TEST_F(StatusTest, APPLY_SIGNAL_HANDLER)
// {
//   // We expect that there is exactly one button that can be used to 'apply' changes to a profile.
//   EXPECT_CALL(*this, handle_signal()).Times(1);

//   auto fun = sigc::mem_fun(*this, &StatusTest::handle_signal);
//   sm.set_apply_signal_handler(fun);

//   click_everything(&sm);
// }

// // Recursive method to check whether label exists with `label_text`
// bool StatusTest::check_label_exists(Gtk::Widget *obj, std::string label_text)
// {
//   // Attempt to cast the object as a Gtk::Label
//   Gtk::Label *lab = dynamic_cast<Gtk::Label *>(obj);

//   // If cb is not a nullptr, then it is a Gtk::Label
//   if(lab) {
//     // click the button
//     return (lab->get_text().compare(label_text) == 0);
//   }

//   // Attempt to cast the object as a Gtk::Container, then enumerate children
//   Gtk::Container *parent = dynamic_cast<Gtk::Container *>(obj);

//   // If parent is not a nullptr, then it is a Gtk::Container
//   if(parent) {
//     auto children = parent->get_children();

//     for(auto child: children) {
//       bool re = check_label_exists(child, label_text);
//       // If there was a child Gtk::Label with label_text as its text
//       if(re) {
//         return true;
//       }
//     }
//   }

//   return false;
// }

// TEST_F(StatusTest, CHECK_APPLY_LABEL_TEXT)
// {
//   std::string label_text = "arbitrary text for apply label";
//   EXPECT_FALSE(check_label_exists(&sm, label_text));
//   sm.set_apply_label_text(label_text);
//   EXPECT_TRUE(check_label_exists(&sm, label_text));
// }

// TEST_F(StatusTest, CHECK_STATUS_LABEL_TEXT)
// {
//   std::string label_text = "arbitrary text for status label";
//   EXPECT_FALSE(check_label_exists(&sm, label_text));
//   sm.set_apply_label_text(label_text);
//   EXPECT_TRUE(check_label_exists(&sm, label_text));
// }

// // Recursive method to set change the selection text of all comboboxes
// void StatusTest::set_combobox_text(Gtk::Widget *obj, std::string combo_text)
// {
//   // Attempt to cast the object as a Gtk::Button, then click it
//   // This will also cast CheckButtons which are a type of Button
//   Gtk::ComboBoxText *cb = dynamic_cast<Gtk::ComboBoxText *>(obj);

//   // If cb is not a nullptr, then it is a Gtk::ComboBoxText
//   if(cb) {
//     // click the button
//     cb->set_active_text(combo_text);
//   }

//   // Attempt to cast the object as a Gtk::Container, then enumerate children
//   Gtk::Container *parent = dynamic_cast<Gtk::Container *>(obj);

//   // If parent is not a nullptr, then it is a Gtk::Container
//   if(parent) {
//     auto children = parent->get_children();

//     for(auto child: children) {
//       set_combobox_text(child, combo_text);
//     }
//   }
// }

// TEST_F(StatusTest, CHECK_GET_SELECTION_TEXT)
// {
//   std::string text = "Enforce";
//   set_combobox_text(&sm, text);
//   EXPECT_EQ(sm.get_selection_text(), text);

//   text = "Complain";
//   set_combobox_text(&sm, text);
//   EXPECT_EQ(sm.get_selection_text(), text);

//   text = "Disable";
//   set_combobox_text(&sm, text);
//   EXPECT_EQ(sm.get_selection_text(), text);
// }

// // Test for method parse_JSON(...) with an argument that is not in valid JSON format
// TEST_F(StatusTest, PARSE_JSON_INVALID_FORMAT)
// {
//   std::string raw_json = "";
//   EXPECT_THROW(sm.parse_JSON(raw_json), std::invalid_argument);
// }

// // Test for method parse_JSON(...) with an argument containing a null value
// TEST_F(StatusTest, PARSE_JSON_NULL_VALUE)
// {
//   std::string raw_json = "{\"testKey\": null}";
//   Json::Value res      = sm.parse_JSON(raw_json);
//   ASSERT_FALSE(res.isNull()) << "result of parsing json should not be null";
//   EXPECT_EQ(res.type(), Json::ValueType::objectValue) << "ValueType of result should be ValueType.objectValue";
//   EXPECT_EQ(res["testKey"].type(), Json::ValueType::nullValue) << "ValueType of property testKey should be ValueType.nullValue";
//   EXPECT_TRUE(res["testKey"].isNull()) << "value of property testKey should be null";
// }

// // Test for method parse_JSON(...) with an argument containing a string value
// TEST_F(StatusTest, PARSE_JSON_STRING_VALUE)
// {
//   std::string raw_json = "{\"testKey\": \"testString\"}";
//   Json::Value res      = sm.parse_JSON(raw_json);
//   ASSERT_FALSE(res.isNull()) << "result of parsing json should not be null";
//   EXPECT_EQ(res.type(), Json::ValueType::objectValue) << "ValueType of result should be ValueType.objectValue";
//   EXPECT_EQ(res["testKey"].type(), Json::ValueType::stringValue) << "ValueType of property testKey should be ValueType.stringValue";
//   EXPECT_EQ(res["testKey"], "testString") << "value of property testKey should be testString";
// }

// // Test for method parse_JSON(...) with an argument containing an int value
// TEST_F(StatusTest, PARSE_JSON_INT_VALUE)
// {
//   std::string raw_json = "{\"testKey\": 10}";
//   Json::Value res      = sm.parse_JSON(raw_json);
//   ASSERT_FALSE(res.isNull()) << "result of parsing json should not be null";
//   EXPECT_EQ(res.type(), Json::ValueType::objectValue) << "ValueType of result should be ValueType.objectValue";
//   EXPECT_EQ(res["testKey"].type(), Json::ValueType::intValue) << "ValueType of property testKey should be ValueType.intValue";
//   EXPECT_EQ(res["testKey"], 10) << "value of property testKey should be 10";
// }

// // Test for method parse_JSON(...) with an argument containing a real (double) value
// TEST_F(StatusTest, PARSE_JSON_REAL_VALUE)
// {
//   std::string raw_json = "{\"testKey\": 5.0}";
//   Json::Value res      = sm.parse_JSON(raw_json);
//   ASSERT_FALSE(res.isNull()) << "result of parsing json should not be null";
//   EXPECT_EQ(res.type(), Json::ValueType::objectValue) << "ValueType of result should be ValueType.objectValue";
//   EXPECT_EQ(res["testKey"].type(), Json::ValueType::realValue) << "ValueType of property testKey should be ValueType.realValue";
//   EXPECT_EQ(res["testKey"], 5.0) << "value of property testKey should be 5.0";
// }

// // Test for method parse_JSON(...) with an argument containing a boolean value
// TEST_F(StatusTest, PARSE_JSON_BOOLEAN_VALUE)
// {
//   std::string raw_json = "{\"testKey\": true}";
//   Json::Value res      = sm.parse_JSON(raw_json);
//   ASSERT_FALSE(res.isNull()) << "result of parsing json should not be null";
//   EXPECT_EQ(res.type(), Json::ValueType::objectValue) << "ValueType of result should be ValueType.objectValue";
//   EXPECT_EQ(res["testKey"].type(), Json::ValueType::booleanValue) << "ValueType of property testKey should be ValueType.booleanValue";
//   EXPECT_EQ(res["testKey"], true) << "value of property testKey should be true";
// }

// // Test for method parse_JSON(...) with an argument containing an array value
// TEST_F(StatusTest, PARSE_JSON_ARRAY_VALUE)
// {
//   std::string raw_json = "{\"testKey\": [\"string1\", \"string2\", \"string3\"]}";
//   Json::Value res      = sm.parse_JSON(raw_json);
//   ASSERT_FALSE(res.isNull()) << "result of parsing json should not be null";
//   EXPECT_EQ(res.type(), Json::ValueType::objectValue) << "ValueType of result should be ValueType.objectValue";
//   EXPECT_EQ(res["testKey"].type(), Json::ValueType::arrayValue) << "ValueType of property testKey should be ValueType.arrayValue";
//   EXPECT_EQ(res["testKey"][0], "string1") << "value of first element of property testKey should be string1";
//   EXPECT_EQ(res["testKey"][1], "string2") << "value of second element of property testKey should be string2";
//   EXPECT_EQ(res["testKey"][2], "string3") << "value of third element of property testKey should be string3";
// }

// // Test for method parse_JSON(...) with an argument containing an object value
// TEST_F(StatusTest, PARSE_JSON_OBJECT_VALUE)
// {
//   std::string raw_json = "{\"testKey\": {\"subKey1\": \"string1\", \"subKey2\": 20, \"subKey3\": true}}";
//   Json::Value res      = sm.parse_JSON(raw_json);
//   ASSERT_FALSE(res.isNull()) << "result of parsing json should not be null";
//   EXPECT_EQ(res.type(), Json::ValueType::objectValue) << "ValueType of result should be ValueType.objectValue";
//   EXPECT_EQ(res["testKey"].type(), Json::ValueType::objectValue) << "ValueType of property testKey should be ValueType.objectValue";
//   EXPECT_EQ(res["testKey"]["subKey1"], "string1") << "value of property subKey1 within testKey should be string1";
//   EXPECT_EQ(res["testKey"]["subKey2"], 20) << "value of property subKey2 within testKey should be 20";
//   EXPECT_EQ(res["testKey"]["subKey3"], true) << "value of property subKey3 within testKey should be true";
// }

// // Test for method parse_JSON(...) with an argument containing a sample snippet of output taken from running
// // 'aa-status --json' in a linux terminal. The passed argument is an example of the types of arguments that the
// // method parse_JSON(...) will receive during actual use
// TEST_F(StatusTest, PARSE_JSON_SAMPLE_INPUT)
// {
//   std::string raw_json =
//       "{\"version\" : \"1\", \"profiles\" : {\"/snap/snapd/14978/usr/lib/snapd/snap-confine\" : \"enforce\", \"/usr/bin/evince\" : "
//       "\"complain\"}, \"processes\": {\"/usr/sbin/cupsd\" : [ {\"profile\" : \"/usr/sbin/cupsd\", \"pid\" : \"616\", \"status\" : "
//       "\"enforce\"} ], \"/usr/sbin/cups-browsed\" : [ {\"profile\" : \"/usr/sbin/NetworkManager\", \"pid\" : \"619\", \"status\" : "
//       "\"unconfined\"} ] }}";
//   Json::Value res = sm.parse_JSON(raw_json);
//   ASSERT_FALSE(res.isNull()) << "result of parsing json should not be null";
//   EXPECT_EQ(res.type(), Json::ValueType::objectValue) << "ValueType of result should be ValueType.objectValue";
//   EXPECT_EQ(res["profiles"].type(), Json::ValueType::objectValue) << "ValueType of property profiles should be ValueType.objectValue";
//   EXPECT_EQ(res["profiles"]["/snap/snapd/14978/usr/lib/snapd/snap-confine"].type(), Json::ValueType::stringValue)
//       << "ValueType of property /snap/snapd/14978/usr/lib/snapd/snap-confine within profiles should be ValueType.stringValue";
//   EXPECT_EQ(res["profiles"]["/snap/snapd/14978/usr/lib/snapd/snap-confine"], "enforce")
//       << "value of property /snap/snapd/14978/usr/lib/snapd/snap-confine should be enforce";
//   EXPECT_EQ(res["profiles"]["/usr/bin/evince"].type(), Json::ValueType::stringValue)
//       << "ValueType of property /usr/bin/evince within profiles should be ValueType.stringValue";
//   EXPECT_EQ(res["profiles"]["/usr/bin/evince"], "complain") << "value of property /usr/bin/evince within profiles should be complain";
//   EXPECT_EQ(res["processes"]["/usr/sbin/cupsd"].type(), Json::ValueType::arrayValue)
//       << "ValueType of property /usr/sbin/cupsd within processes should be ValueType.arrayValue";
//   EXPECT_EQ(res["processes"]["/usr/sbin/cupsd"][0]["pid"], "616")
//       << "value of property pid within /usr/sbin/cupsd in processes should be 616";
//   EXPECT_EQ(res["processes"]["/usr/sbin/cupsd"][0]["status"], "enforce")
//       << "value of property status within /usr/sbin/cupsd in processes should be enforce";
//   EXPECT_EQ(res["processes"]["/usr/sbin/cups-browsed"].type(), Json::ValueType::arrayValue)
//       << "ValueType of property /usr/sbin/cups-browsed within processes should be ValueType.arrayValue";
//   EXPECT_EQ(res["processes"]["/usr/sbin/cups-browsed"][0]["pid"], "619")
//       << "value of property pid within /usr/sbin/cups-browsed in processes should be 619";
//   EXPECT_EQ(res["processes"]["/usr/sbin/cups-browsed"][0]["status"], "unconfined")
//       << "value of property status within /usr/sbin/cups-browsed in processes should be unconfined";
// }
