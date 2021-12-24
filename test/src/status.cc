#include <gtest/gtest.h>

#include "../../src/tabs/status.h"

class StatusTest : public Status
{
	FRIEND_TEST(STATUS, FILTER_FFF);
	FRIEND_TEST(STATUS, FILTER_TFF);
	FRIEND_TEST(STATUS, FILTER_FTF);
	FRIEND_TEST(STATUS, FILTER_FFT);
	FRIEND_TEST(STATUS, FILTER_TTF);
	FRIEND_TEST(STATUS, FILTER_TFT);
	FRIEND_TEST(STATUS, FILTER_FTT);
	FRIEND_TEST(STATUS, FILTER_TTT);
	FRIEND_TEST(STATUS, FILTER_REGEX_TOLOWER);
};

// FILTER tests by: Zixin Yang
TEST(STATUS, FILTER_FFF){
	bool res = StatusTest::filter("abcdEFGH", "cDEf", false, false, false);
	//bool res = testStatus::wrapfilter("abcdEFGH", "cDEf", false, false, false);
	EXPECT_TRUE(res);

	res = StatusTest::filter("abcdEFGH", "gHj", false, false, false);
	EXPECT_FALSE(res);
}

TEST(STATUS, FILTER_TFF){
	bool res = StatusTest::filter("abcdEFGH", "c*f", true, false, false);
 	EXPECT_TRUE(res);

	res = StatusTest::filter("abcdEFGH", "g*j", true, false, false);
	EXPECT_FALSE(res);
}

TEST(STATUS, FILTER_FTF){
	bool res = StatusTest::filter("abcdEFGH", "cdEF", false, true, false);
	EXPECT_TRUE(res);

	res = StatusTest::filter("abcdEFGH", "cdef", false, true, false);
	EXPECT_FALSE(res);
}

TEST(STATUS, FILTER_FFT){
	bool res = StatusTest::filter("abcdEFGH", "abcdEFGh", false, false, true);
	EXPECT_TRUE(res);

	res = StatusTest::filter("abcdEFGH", "abcdEFG", false, false, true);
	EXPECT_FALSE(res);
}

TEST(STATUS, FILTER_TTF){
	bool res = StatusTest::filter("abcdEFGH", "c.*F", true, true, false);
	EXPECT_TRUE(res);

	res = StatusTest::filter("abcdEFGH", "c.*f", true, true, false);
	EXPECT_FALSE(res);
}

TEST(STATUS, FILTER_TFT){
	bool res = StatusTest::filter("abcdEFGH", "[a-z]{8}", true, false, true);
	EXPECT_TRUE(res);

	res = StatusTest::filter("abcdEFGH", "[a-z]{7}", true, false, true);
	EXPECT_FALSE(res);

}

TEST(STATUS, FILTER_FTT){
	bool res = StatusTest::filter("abcdEFGH", "abcdEFGH", false, true, true);
	EXPECT_TRUE(res);

	res = StatusTest::filter("abcdEFGH", "ABCDEFGH", false, true, true);
	EXPECT_FALSE(res);
}

TEST(STATUS, FILTER_TTT){
	bool res = StatusTest::filter("abcdEFGH", "\\w{8}", true, true, true);
	EXPECT_TRUE(res);

	res = StatusTest::filter("abcdEFGH", "\\w{7}", true, true, true);
	EXPECT_FALSE(res);
}

TEST(STATUS, FILTER_REGEX_TOLOWER){
	bool res = StatusTest::filter("abcdefgh ", "\\S{8}", true, true, false);
	EXPECT_TRUE(res);

	res = StatusTest::filter("abcdefgh ", "\\S{8}", true, false, false);		// regex toLower() cause problem
	EXPECT_TRUE(res);
}
