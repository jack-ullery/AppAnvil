#include "../../../src/threads/blocking_queue.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

template<class T, class Deque, class Mutex> class BlockingQueueMock : public BlockingQueue<T, Deque, Mutex>
{
public:
  FRIEND_TEST(BlockingQueueTest, TEST_FRONT);
  FRIEND_TEST(BlockingQueueTest, TEST_BACK);
  FRIEND_TEST(BlockingQueueTest, TEST_SIZE);
  FRIEND_TEST(BlockingQueueTest, TEST_EMPTY);
  FRIEND_TEST(BlockingQueueTest, TEST_CLEAR);
  FRIEND_TEST(BlockingQueueTest, TEST_PUSH);
  FRIEND_TEST(BlockingQueueTest, TEST_PUSH_FRONT);
  FRIEND_TEST(BlockingQueueTest, TEST_POP);

  explicit BlockingQueueMock(std::shared_ptr<Deque> my_internal_queue, std::shared_ptr<Mutex> my_mtx)
      : BlockingQueue<T, Deque, Mutex>(my_internal_queue, my_mtx)
  {
  }
};
