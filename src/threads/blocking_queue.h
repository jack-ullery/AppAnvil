#ifndef SRC_THREADS_BLOCKING_QUEUE_H
#define SRC_THREADS_BLOCKING_QUEUE_H

#include <deque>
#include <memory>
#include <mutex>

// If the unit tests are enabled, include the following header
#ifdef TESTS_ENABLED
#include <gtest/gtest.h>
#endif

/**
 * This is a thread safe queue that blocks on a mutex for each operation
 **/
template<class T, class Deque, class Mutex>
class BlockingQueue
{
public:
  // Constructor
  BlockingQueue();
  // For unit testing
  explicit BlockingQueue(std::shared_ptr<Deque> my_internal_queue, std::shared_ptr<Mutex> my_mtx);

  // Accessor Methods
  T front();
  T back();
  int size();
  bool empty();

  // Mutator Methods
  void clear();
  void push(const T &value);
  void push_front(const T &value);
  T pop();

private:
  // The internal queue that this class acts as a wrapper for
  std::shared_ptr<Deque> internal_queue;
  // A mutex to lock the queue when performing operations
  std::shared_ptr<Mutex> mtx;

#ifdef TESTS_ENABLED
  FRIEND_TEST(BlockingQueueTest, TEST_FRONT);
  FRIEND_TEST(BlockingQueueTest, TEST_BACK);
  FRIEND_TEST(BlockingQueueTest, TEST_SIZE);
  FRIEND_TEST(BlockingQueueTest, TEST_EMPTY);
  FRIEND_TEST(BlockingQueueTest, TEST_CLEAR);
  FRIEND_TEST(BlockingQueueTest, TEST_PUSH);
  FRIEND_TEST(BlockingQueueTest, TEST_PUSH_FRONT);
  FRIEND_TEST(BlockingQueueTest, TEST_POP);
#endif
};

#endif // SRC_THREADS_BLOCKING_QUEUE_H
