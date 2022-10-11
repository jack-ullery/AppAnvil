#include "blocking_queue.h"
#include "deque_mock.h"
#include "mutex_mock.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::Sequence;

// Expect the mutex to be locked and unlocked `num` times
// If lock/unlock is called in the wrong order this throws an error
// Fuerthermore, `mtx_mock` is a fake mutex which never blocks.
void BlockingQueueTest::expect_locks(unsigned int num)
{
  Sequence lock_calls;

  for (unsigned int i = 0; i < num; i++) {
    EXPECT_CALL(*mtx_mock, lock()).Times(1).InSequence(lock_calls);
    EXPECT_CALL(*mtx_mock, unlock()).Times(1).InSequence(lock_calls);
  }
}

// Test for method front()
TEST_F(BlockingQueueTest, TEST_FRONT)
{
  EXPECT_CALL(*my_internal_queue, front()).Times(1);
  expect_locks(1);
  b_queue.front();
}

// Test for method back()
TEST_F(BlockingQueueTest, TEST_BACK)
{
  EXPECT_CALL(*my_internal_queue, back()).Times(1);
  expect_locks(1);
  b_queue.back();
}

// Test for method size()
TEST_F(BlockingQueueTest, TEST_SIZE)
{
  EXPECT_CALL(*my_internal_queue, size()).Times(1);
  expect_locks(1);
  b_queue.size();
}

// Test for method empty()
TEST_F(BlockingQueueTest, TEST_EMPTY)
{
  EXPECT_CALL(*my_internal_queue, empty()).Times(1);
  expect_locks(1);
  b_queue.empty();
}

// Test for method clear()
TEST_F(BlockingQueueTest, TEST_CLEAR)
{
  EXPECT_CALL(*my_internal_queue, clear()).Times(1);
  expect_locks(1);
  b_queue.clear();
}

// Test for method push(...)
TEST_F(BlockingQueueTest, TEST_PUSH)
{
  EXPECT_CALL(*my_internal_queue, push_back(arg0)).Times(1);
  expect_locks(1);
  b_queue.push(arg0);
}

// Test for method push_front(...)
TEST_F(BlockingQueueTest, TEST_PUSH_FRONT)
{
  EXPECT_CALL(*my_internal_queue, push_front(arg0)).Times(1);
  expect_locks(1);
  b_queue.push_front(arg0);
}

// Test for method pop()
TEST_F(BlockingQueueTest, TEST_POP)
{
  EXPECT_CALL(*my_internal_queue, front()).Times(1);
  EXPECT_CALL(*my_internal_queue, pop_front()).Times(1);
  expect_locks(1);
  b_queue.pop();
}