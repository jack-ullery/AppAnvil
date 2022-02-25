#include "../../../src/threads/blocking_queue.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

template<class T, class Deque, class Mutex> class BlockingQueueMock : public BlockingQueue<T, Deque, Mutex>
{
public:
  FRIEND_TEST(BlockingQueueTest, TEST_FRONT_SINGLE_ELEMENT);
  FRIEND_TEST(BlockingQueueTest, TEST_FRONT_MANY_ELEMENTS);
  FRIEND_TEST(BlockingQueueTest, TEST_BACK_SINGLE_ELEMENT);
  FRIEND_TEST(BlockingQueueTest, TEST_BACK_MANY_ELEMENTS);
  FRIEND_TEST(BlockingQueueTest, TEST_SIZE_INITIAL);
  FRIEND_TEST(BlockingQueueTest, TEST_SIZE_AFTER_SINGLE_PUSH_POP);
  FRIEND_TEST(BlockingQueueTest, TEST_SIZE_AFTER_MANY_PUSH_POP);
  FRIEND_TEST(BlockingQueueTest, TEST_EMPTY_INITIAL);
  FRIEND_TEST(BlockingQueueTest, TEST_EMPTY_AFTER_PUSH_BACK_AND_POP);
  FRIEND_TEST(BlockingQueueTest, TEST_EMPTY_AFTER_PUSH_FRONT_AND_POP);
  FRIEND_TEST(BlockingQueueTest, TEST_EMPTY_AFTER_MULTIPLE_PUSH_POP);
  FRIEND_TEST(BlockingQueueTest, TEST_CLEAR_SINGLE_ELEMENT);
  FRIEND_TEST(BlockingQueueTest, TEST_CLEAR_MANY_ELEMENTS);
  FRIEND_TEST(BlockingQueueTest, TEST_PUSH_ELEMENT_SEQUENCE);
  FRIEND_TEST(BlockingQueueTest, TEST_PUSH_FRONT_ELEMENT_SEQUENCE);
  FRIEND_TEST(BlockingQueueTest, TEST_POP_SINGLE_ELEMENT);
  FRIEND_TEST(BlockingQueueTest, TEST_POP_MANY_ELEMENTS);

  explicit BlockingQueueMock(Deque my_internal_queue, std::shared_ptr<Mutex> my_mtx)
      : BlockingQueue<T, Deque, Mutex>(my_internal_queue, my_mtx)
  {
  }
};
