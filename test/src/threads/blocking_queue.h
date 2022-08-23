#ifndef TEST_SRC_THREADS_BLOCKING_QUEUE_H
#define TEST_SRC_THREADS_BLOCKING_QUEUE_H

#include "../../../src/threads/blocking_queue.cc"
#include "deque_mock.h"
#include "mutex_mock.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

// Test Fixture for BlockingQueue class
class BlockingQueueTest : public ::testing::Test
{
protected:
  BlockingQueueTest()
    : my_internal_queue{ new DequeMock<int>() },
      mtx_mock{ new MutexMock() },
      b_queue(my_internal_queue, mtx_mock)
  {
  }

  virtual void SetUp() {}

  void expect_locks(unsigned int num);

  const int arg0 = 0;

  std::shared_ptr<DequeMock<int>> my_internal_queue;
  std::shared_ptr<MutexMock> mtx_mock;

  BlockingQueue<int, DequeMock<int>, MutexMock> b_queue;
};

// Used to avoid linker errors
// For more information, see: https://isocpp.org/wiki/faq/templates#class-templates
template class BlockingQueue<int, DequeMock<int>, MutexMock>;

#endif