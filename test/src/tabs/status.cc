#include "status_mock.cc"

#include <chrono>
#include <thread>

class StatusTest : public ::testing::Test
{
public:
	MOCK_METHOD(void, handle_signal, ());

protected:
	StatusTest(){}
	virtual void SetUp(){}

	void click_everything(Gtk::Widget *obj);

	StatusMock sm;
};

TEST_F(StatusTest, FILTER_FFF){
	bool res = StatusMock::filter("abcdEFGH", "cDEf", false, false, false);
	EXPECT_TRUE(res);

	res = StatusMock::filter("abcdEFGH", "gHj", false, false, false);
	EXPECT_FALSE(res);
}

TEST_F(StatusTest, FILTER_TFF){
	bool res = StatusMock::filter("abcdEFGH", "c*f", true, false, false);
 	EXPECT_TRUE(res);

	res = StatusMock::filter("abcdEFGH", "g*j", true, false, false);
	EXPECT_FALSE(res);
}

TEST_F(StatusTest, FILTER_FTF){
	bool res = StatusMock::filter("abcdEFGH", "cdEF", false, true, false);
	EXPECT_TRUE(res);

	res = StatusMock::filter("abcdEFGH", "cdef", false, true, false);
	EXPECT_FALSE(res);
}

TEST_F(StatusTest, FILTER_FFT){
	bool res = StatusMock::filter("abcdEFGH", "abcdEFGh", false, false, true);
	EXPECT_TRUE(res);

	res = StatusMock::filter("abcdEFGH", "abcdEFG", false, false, true);
	EXPECT_FALSE(res);
}

TEST_F(StatusTest, FILTER_TTF){
	bool res = StatusMock::filter("abcdEFGH", "c.*F", true, true, false);
	EXPECT_TRUE(res);

	res = StatusMock::filter("abcdEFGH", "c.*f", true, true, false);
	EXPECT_FALSE(res);
}

TEST_F(StatusTest, FILTER_TFT){
	bool res = StatusMock::filter("abcdEFGH", "[a-z]{8}", true, false, true);
	EXPECT_TRUE(res);

	res = StatusMock::filter("abcdEFGH", "[a-z]{7}", true, false, true);
	EXPECT_FALSE(res);
}

TEST_F(StatusTest, FILTER_FTT){
	bool res = StatusMock::filter("abcdEFGH", "abcdEFGH", false, true, true);
	EXPECT_TRUE(res);

	res = StatusMock::filter("abcdEFGH", "ABCDEFGH", false, true, true);
	EXPECT_FALSE(res);
}

TEST_F(StatusTest, FILTER_TTT){
	bool res = StatusMock::filter("abcdEFGH", "\\w{8}", true, true, true);
	EXPECT_TRUE(res);

	res = StatusMock::filter("abcdEFGH", "\\w{7}", true, true, true);
	EXPECT_FALSE(res);
}

TEST_F(StatusTest, FILTER_REGEX_TOLOWER){
	bool res = StatusMock::filter("abcdefgh ", "\\S{8}", true, true, false);
	EXPECT_TRUE(res);

	res = StatusMock::filter("abcdefgh ", "\\S{8}", true, false, false);
	EXPECT_TRUE(res);
}

TEST_F(StatusTest, VIEW_NOT_NULL){
	// We know `nullptr` evaluates to `0` which is `FALSE`
	// This check `sm->get_view` does not return a null pointer
	EXPECT_TRUE(sm.get_view()) << "get_view() unexpectedly returned a nullptr.";
}

// Recursive method to click all the checkboxes that are descendents of a Container Widget
void StatusTest::click_everything(Gtk::Widget *obj){
	// Attempt to cast the object as a Gtk::Button, then click it
	// This will also cast CheckButtons which are a type of Button
	Gtk::Button *bu  = dynamic_cast<Gtk::Button*>(obj);
	// If cb is not a nullptr, then it is a Gtk::Button
	if(bu){
		// click the button
		bu->clicked();
	}

	// Attempt to cast the object as a Gtk::Container, then enumerate children
	Gtk::Container *parent = dynamic_cast<Gtk::Container*>(obj);
	// If parent is not a nullptr, then it is a Gtk::Container
	if(parent){
		auto children = parent->get_children();
		for(auto child : children){
			click_everything(child);
		}
	}
}

TEST_F(StatusTest, REFRESH_SIGNAL_HANDLER){
	// We know there are exactly three CheckButtons and one SearchEntry that can be modified to send a signal
	// However, I was getting some problems emitting the `signal_search_changed` event, so this does not test the SearchEntry
	// If there are a different amount of CheckButtons, this will fail.
	EXPECT_CALL(*this, handle_signal()).Times(3);

	auto fun = sigc::mem_fun(*this, &StatusTest::handle_signal);
	sm.set_refresh_signal_handler(fun);
	
	click_everything(&sm);
}

TEST_F(StatusTest, APPLY_SIGNAL_HANDLER){
	// We expect that there is exactly one button that can be used to 'apply' changes to a profile.
	EXPECT_CALL(*this, handle_signal()).Times(1);

	auto fun = sigc::mem_fun(*this, &StatusTest::handle_signal);
	sm.set_apply_signal_handler(fun);

	click_everything(&sm);
}
