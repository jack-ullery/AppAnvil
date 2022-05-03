#ifndef TEST_SRC_THREADS_DEQUEUE_MOCK_H
#define TEST_SRC_THREADS_DEQUEUE_MOCK_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>

template<class T> class DequeMock
{
public:
  MOCK_METHOD(T, front, ());
  MOCK_METHOD(T, back, ());
  MOCK_METHOD(int, size, ());
  MOCK_METHOD(bool, empty, ());
  MOCK_METHOD(void, clear, ());
  MOCK_METHOD(void, push_back, (const T &value));
  MOCK_METHOD(void, push_front, (const T &value));
  MOCK_METHOD(T, pop_front, ());

  DequeMock() { }
  DequeMock(const DequeMock &) { }
};

#endif