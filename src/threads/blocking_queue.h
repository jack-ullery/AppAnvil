#ifndef SRC_THREADS_BLOCKING_QUEUE_H
#define SRC_THREADS_BLOCKING_QUEUE_H

#include <deque>
#include <memory>
#include <mutex>

/** 
  * This is a thread safe queue that blocks on a mutex for each operation
  **/
template<class T, class Deque, class Mutex> class BlockingQueue
{
public:
  // Constructor
  BlockingQueue();
  // For unit testing
  //explicit BlockingQueue(std::deque<T> my_internal_queue, std::shared_ptr<Mutex> my_mtx);
  explicit BlockingQueue(Deque my_internal_queue, std::shared_ptr<Mutex> my_mtx);

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
  //std::deque<T> internal_queue;
  Deque internal_queue;
  // A mutex to lock the queue when performing operations
  std::shared_ptr<Mutex> mtx;
};

/**
 * Important Note: Because of how the C++ compiler links code with templates, we need to have implementation
 * in the header file
 **/
#include "blocking_queue.inl"

#endif // SRC_THREADS_BLOCKING_QUEUE_H
