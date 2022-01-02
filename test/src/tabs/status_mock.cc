#include <gtest/gtest.h>

#include "../../../src/tabs/status.h"

class StatusMock : public Status
{
	FRIEND_TEST(Status, FILTER_FFF);
	FRIEND_TEST(Status, FILTER_TFF);
	FRIEND_TEST(Status, FILTER_FTF);
	FRIEND_TEST(Status, FILTER_FFT);
	FRIEND_TEST(Status, FILTER_TTF);
	FRIEND_TEST(Status, FILTER_TFT);
	FRIEND_TEST(Status, FILTER_FTT);
	FRIEND_TEST(Status, FILTER_TTT);
	FRIEND_TEST(Status, FILTER_REGEX_TOLOWER);
};
