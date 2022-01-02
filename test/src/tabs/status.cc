#include "status_mock.cc"

TEST(Status, FILTER_FFF){
	bool res = StatusMock::filter("abcdEFGH", "cDEf", false, false, false);
	EXPECT_TRUE(res);

	res = StatusMock::filter("abcdEFGH", "gHj", false, false, false);
	EXPECT_FALSE(res);
}

TEST(Status, FILTER_TFF){
	bool res = StatusMock::filter("abcdEFGH", "c*f", true, false, false);
 	EXPECT_TRUE(res);

	res = StatusMock::filter("abcdEFGH", "g*j", true, false, false);
	EXPECT_FALSE(res);
}

TEST(Status, FILTER_FTF){
	bool res = StatusMock::filter("abcdEFGH", "cdEF", false, true, false);
	EXPECT_TRUE(res);

	res = StatusMock::filter("abcdEFGH", "cdef", false, true, false);
	EXPECT_FALSE(res);
}

TEST(Status, FILTER_FFT){
	bool res = StatusMock::filter("abcdEFGH", "abcdEFGh", false, false, true);
	EXPECT_TRUE(res);

	res = StatusMock::filter("abcdEFGH", "abcdEFG", false, false, true);
	EXPECT_FALSE(res);
}

TEST(Status, FILTER_TTF){
	bool res = StatusMock::filter("abcdEFGH", "c.*F", true, true, false);
	EXPECT_TRUE(res);

	res = StatusMock::filter("abcdEFGH", "c.*f", true, true, false);
	EXPECT_FALSE(res);
}

TEST(Status, FILTER_TFT){
	bool res = StatusMock::filter("abcdEFGH", "[a-z]{8}", true, false, true);
	EXPECT_TRUE(res);

	res = StatusMock::filter("abcdEFGH", "[a-z]{7}", true, false, true);
	EXPECT_FALSE(res);

}

TEST(Status, FILTER_FTT){
	bool res = StatusMock::filter("abcdEFGH", "abcdEFGH", false, true, true);
	EXPECT_TRUE(res);

	res = StatusMock::filter("abcdEFGH", "ABCDEFGH", false, true, true);
	EXPECT_FALSE(res);
}

TEST(Status, FILTER_TTT){
	bool res = StatusMock::filter("abcdEFGH", "\\w{8}", true, true, true);
	EXPECT_TRUE(res);

	res = StatusMock::filter("abcdEFGH", "\\w{7}", true, true, true);
	EXPECT_FALSE(res);
}

TEST(Status, FILTER_REGEX_TOLOWER){
	bool res = StatusMock::filter("abcdefgh ", "\\S{8}", true, true, false);
	EXPECT_TRUE(res);

	res = StatusMock::filter("abcdefgh ", "\\S{8}", true, false, false);		// regex toLower() cause problem
	EXPECT_TRUE(res);
}
