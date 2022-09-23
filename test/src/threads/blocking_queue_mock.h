#ifndef TEST_SRC_THREADS_BLOCKING_QUEUE_MOCK_H
#define TEST_SRC_THREADS_BLOCKING_QUEUE_MOCK_H

#include "../../../src/threads/blocking_queue.cc"
#include "deque_mock.h"
#include "mutex_mock.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

template<class T, class Deque, class Mutex>
class BlockingQueueMock : public BlockingQueue<T, Deque, Mutex>
{
public:
  explicit BlockingQueueMock(std::shared_ptr<Deque> my_internal_queue, std::shared_ptr<Mutex> my_mtx)
    : BlockingQueue<T, Deque, Mutex>(my_internal_queue, my_mtx)
  {
  }
};

#endif