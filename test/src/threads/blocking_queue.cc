#include "./blocking_queue_mock.cc"
#include "./mutex_mock.cc"
#include "./deque_mock.cc"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::Sequence;

class BlockingQueueTest : public ::testing::Test
{
protected:
  BlockingQueueTest() : my_internal_queue(), mtx_mock{new MutexMock()}, b_queue(my_internal_queue, mtx_mock) { }

  virtual void SetUp() { }

  void expect_locks(unsigned int num);

  const int arg0     = 0;
  const int arg1     = 1;
  const int CAPACITY = 100;

  //std::deque<int> my_internal_queue;
  DequeMock<int> my_internal_queue;
  std::shared_ptr<MutexMock> mtx_mock;

  // Note that I decided to use the int type when creating the tests for the sake of simplicity, however this should probably be changed in the future.
  // Blocking queue is utilized by only two other files I think, namely dispatcher_middleman and console_thread. In order to make the tests more closely
  // resemble the performance environment, the int type should be swapped out for the actual types that are used during runtime.
  // For dispatcher_middleman, the type passed is CallData, which is a struct containing CallType (an enum) and two strings
  // For console_thread, the type passed is Message, which is a struct containing an Event (an enum), a TabState (another enum), and a vector of strings
  BlockingQueueMock<int, MutexMock> b_queue;
};

// Expect the mutex to be locked and unlocked `num` times
// If lock/unlock is called in the wrong order this throws an error
// Fuerthermore, `mtx_mock` is a fake mutex which never blocks.
void BlockingQueueTest::expect_locks(unsigned int num)
{
  Sequence lock_calls;

  for(unsigned int i = 0; i < num; i++) {
    EXPECT_CALL(*mtx_mock, lock()).Times(1).InSequence(lock_calls);

    EXPECT_CALL(*mtx_mock, unlock()).Times(1).InSequence(lock_calls);
  }
}

TEST_F(BlockingQueueTest, TEST_FRONT_SINGLE_ELEMENT)
{
  expect_locks(2);
  EXPECT_CALL(my_internal_queue, push_back(arg0)).Times(1);
  EXPECT_CALL(my_internal_queue, front()).Times(1);
  b_queue.push(arg0);
  auto val = b_queue.front();
  EXPECT_EQ(val, arg0) << "front() should return 0 after a single element with value 0 is pushed";
}

TEST_F(BlockingQueueTest, TEST_FRONT_MANY_ELEMENTS)
{
  expect_locks(CAPACITY * 2);
  for(int i = 0; i < CAPACITY; i++) {
    b_queue.push(i);
    auto val = b_queue.front();
    EXPECT_EQ(val, 0) << "front() should return 0 after " + std::to_string(i + 1) + " elements have been pushed";
  }
}

TEST_F(BlockingQueueTest, TEST_BACK_SINGLE_ELEMENT)
{
  expect_locks(2);
  b_queue.push(arg0);
  auto val = b_queue.back();
  EXPECT_EQ(val, arg0) << "back() should return 0 after a single element with value 0 is pushed";
}

TEST_F(BlockingQueueTest, TEST_BACK_MANY_ELEMENTS)
{
  expect_locks(CAPACITY * 2);
  for(int i = 0; i < CAPACITY; i++) {
    b_queue.push(i);
    auto val = b_queue.back();
    EXPECT_EQ(val, i) << "back() should return " + std::to_string(i) + " after " + std::to_string(i + 1) + " elements have been pushed";
  }
}

TEST_F(BlockingQueueTest, TEST_SIZE_INITIAL)
{
  expect_locks(1);
  int s = b_queue.size();
  EXPECT_EQ(s, 0) << "size() should return 0 after queue is initialized";
}

TEST_F(BlockingQueueTest, TEST_SIZE_AFTER_SINGLE_PUSH_POP)
{
  expect_locks(4);
  b_queue.push(arg0);
  int s = b_queue.size();
  EXPECT_EQ(s, 1) << "size() should return 1 after a single element has been pushed";
  b_queue.pop();
  s = b_queue.size();
  EXPECT_EQ(s, 0) << "size() should return 0 after a single element has been pushed and popped";
}

TEST_F(BlockingQueueTest, TEST_SIZE_AFTER_MANY_PUSH_POP)
{
  expect_locks(CAPACITY * 4);
  for(int i = 0; i < CAPACITY; i++) {
    b_queue.push(i);
    int s = b_queue.size();
    EXPECT_EQ(s, i + 1) << "size() should return " + std::to_string(i + 1) + " after " + std::to_string(i + 1) +
                               " elements have been pushed";
  }
  for(int i = 0; i < CAPACITY; i++) {
    b_queue.pop();
    int s = b_queue.size();
    EXPECT_EQ(s, CAPACITY - (i + 1)) << "size() should return " + std::to_string(CAPACITY - (i + 1)) + " after " +
                                            std::to_string(CAPACITY) + " elements have been pushed and " + std::to_string(i + 1) +
                                            " elements have been popped";
  }
}

TEST_F(BlockingQueueTest, TEST_EMPTY_INITIAL)
{
  expect_locks(1);
  bool res = b_queue.empty();
  EXPECT_TRUE(res) << "empty() should return true after deque is initialized";
}

TEST_F(BlockingQueueTest, TEST_EMPTY_AFTER_PUSH_BACK_AND_POP)
{
  expect_locks(4);
  b_queue.push(1);
  bool res = b_queue.empty();
  EXPECT_FALSE(res) << "empty() should return false after 1 element is pushed to back";
  b_queue.pop();
  res = b_queue.empty();
  EXPECT_TRUE(res) << "empty() should return true after 1 element is pushed to back and 1 element is popped";
}

TEST_F(BlockingQueueTest, TEST_EMPTY_AFTER_PUSH_FRONT_AND_POP)
{
  expect_locks(4);
  b_queue.push_front(1);
  bool res = b_queue.empty();
  EXPECT_FALSE(res) << "empty() should return false after 1 element is pushed to front";
  b_queue.pop();
  res = b_queue.empty();
  EXPECT_TRUE(res) << "empty() should return true after 1 element is pushed to front and 1 element is popped";
}

TEST_F(BlockingQueueTest, TEST_EMPTY_AFTER_MULTIPLE_PUSH_POP)
{
  expect_locks(6);
  b_queue.push_front(1);
  b_queue.push(2);
  bool res = b_queue.empty();
  EXPECT_FALSE(res) << "empty() should return false after 2 elements have been pushed";
  b_queue.pop();
  b_queue.pop();
  res = b_queue.empty();
  EXPECT_TRUE(res) << "empty() should return true after 2 elements have been pushed and 2 elements have popped";
}

TEST_F(BlockingQueueTest, TEST_CLEAR_SINGLE_ELEMENT)
{
  expect_locks(5);
  b_queue.push(arg0);
  int s = b_queue.size();
  EXPECT_EQ(s, 1) << "size() should return 1 after a single element has been pushed";
  b_queue.clear();
  s = b_queue.size();
  EXPECT_EQ(s, 0) << "size() should return 0 after clear() has been called on a deque with a single element";
  bool res = b_queue.empty();
  EXPECT_TRUE(res) << "empty() should return true after clear() has been called on a deque with a single element";
}

TEST_F(BlockingQueueTest, TEST_CLEAR_MANY_ELEMENTS)
{
  expect_locks(CAPACITY + 4);
  for(int i = 0; i < CAPACITY; i++) {
    b_queue.push(i);
  }
  int s = b_queue.size();
  EXPECT_EQ(s, CAPACITY) << "size() should return " + std::to_string(CAPACITY) + " after " + std::to_string(CAPACITY) +
                                " elements have been pushed";
  b_queue.clear();
  s = b_queue.size();
  EXPECT_EQ(s, 0) << "size() should return 0 after calling clear() on a deque with " + std::to_string(CAPACITY) + " elements";
  bool res = b_queue.empty();
  EXPECT_TRUE(res) << "empty() should return true after calling clear() on a deque with " + std::to_string(CAPACITY) + " elements";
}

TEST_F(BlockingQueueTest, TEST_PUSH_ELEMENT_SEQUENCE)
{
  int intsToAdd[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  int arrLength     = sizeof(intsToAdd) / sizeof(intsToAdd[0]);
  expect_locks(arrLength * 3);
  for(int i = 0; i < arrLength; i++) {
    b_queue.push(intsToAdd[i]);
    int s = b_queue.size();
    EXPECT_EQ(s, i + 1) << "size() should return " + std::to_string(i + 1) + " after pushing " + std::to_string(i + 1) + " elements";
    auto val = b_queue.back();
    EXPECT_EQ(val, intsToAdd[i]) << "back() should return " + std::to_string(intsToAdd[i]) + " after pushing " + std::to_string(i + 1) +
                                        " elements";
  }
}

TEST_F(BlockingQueueTest, TEST_PUSH_FRONT_ELEMENT_SEQUENCE)
{
  int intsToAdd[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  int arrLength     = sizeof(intsToAdd) / sizeof(intsToAdd[0]);
  expect_locks(arrLength * 3);
  for(int i = 0; i < arrLength; i++) {
    b_queue.push_front(intsToAdd[i]);
    int s = b_queue.size();
    EXPECT_EQ(s, i + 1) << "size() should return " + std::to_string(i + 1) + " after pushing " + std::to_string(i + 1) + " elements";
    auto val = b_queue.front();
    EXPECT_EQ(val, intsToAdd[i]) << "front() should return " + std::to_string(intsToAdd[i]) + " after pushing " + std::to_string(i + 1) +
                                        " elements";
  }
}

TEST_F(BlockingQueueTest, TEST_POP_SINGLE_ELEMENT)
{
  expect_locks(2);
  b_queue.push(arg0);
  auto val = b_queue.pop();
  EXPECT_EQ(val, arg0) << "pop() should return 0 when a single element with value 0 has been pushed";
}

TEST_F(BlockingQueueTest, TEST_POP_MANY_ELEMENTS)
{
  expect_locks(CAPACITY * 2);
  for(int i = 0; i < CAPACITY; i++) {
    b_queue.push(i);
  }
  for(int i = 0; i < CAPACITY; i++) {
    auto val = b_queue.pop();
    EXPECT_EQ(val, i) << "pop() should return " + std::to_string(i) + " after pushing " + std::to_string(CAPACITY) +
                             " elements and popping " + std::to_string(i + 1) + " elements";
  }
}
