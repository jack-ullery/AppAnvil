#ifndef TEST_SRC_THREADS_MUTEX_MOCK_H
#define TEST_SRC_THREADS_MUTEX_MOCK_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>

class MutexMock
{
public:
  MOCK_METHOD(void, lock, ());
  MOCK_METHOD(void, unlock, ());
};

#endif