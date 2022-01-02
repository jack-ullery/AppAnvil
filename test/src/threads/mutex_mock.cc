#include <gmock/gmock.h>
#include <gtest/gtest.h>

class MutexMock
{
public:
    MOCK_METHOD(void, lock, ());
    MOCK_METHOD(void, unlock, ());
};
