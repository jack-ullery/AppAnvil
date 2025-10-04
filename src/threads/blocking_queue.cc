#include "blocking_queue.h"
#include "../console_thread.h"

#include <memory>

template<class T, class Mutex, class Deque>
BlockingQueue<T, Mutex, Deque>::BlockingQueue()
  : internal_queue{ std::make_shared<Deque>() },
    mtx{ std::make_shared<Mutex>() }
{
}

template<class T, class Mutex, class Deque>
BlockingQueue<T, Mutex, Deque>::BlockingQueue(std::shared_ptr<Deque> queue, std::shared_ptr<Mutex> my_mtx)
  : internal_queue{ std::move(queue) },
    mtx{ std::move(my_mtx) }
{
}

// Accessor Methods
template<class T, class Mutex, class Deque>
T BlockingQueue<T, Mutex, Deque>::front()
{
  std::lock_guard<Mutex> lock(*mtx);
  return internal_queue->front();
}

template<class T, class Mutex, class Deque>
T BlockingQueue<T, Mutex, Deque>::back()
{
  std::lock_guard<Mutex> lock(*mtx);
  return internal_queue->back();
}

template<class T, class Mutex, class Deque>
int BlockingQueue<T, Mutex, Deque>::size()
{
  std::lock_guard<Mutex> lock(*mtx);
  return internal_queue->size();
}

template<class T, class Mutex, class Deque>
bool BlockingQueue<T, Mutex, Deque>::empty()
{
  std::lock_guard<Mutex> lock(*mtx);
  return internal_queue->empty();
}

// Mutator Methods
template<class T, class Mutex, class Deque>
void BlockingQueue<T, Mutex, Deque>::clear()
{
  std::lock_guard<Mutex> lock(*mtx);
  internal_queue->clear();
}

template<class T, class Mutex, class Deque>
void BlockingQueue<T, Mutex, Deque>::push(const T &value)
{
  std::lock_guard<Mutex> lock(*mtx);
  internal_queue->push_back(value);
}

template<class T, class Mutex, class Deque>
void BlockingQueue<T, Mutex, Deque>::push_front(const T &value)
{
  std::lock_guard<Mutex> lock(*mtx);
  internal_queue->push_front(value);
}

template<class T, class Mutex, class Deque>
T BlockingQueue<T, Mutex, Deque>::pop()
{
  std::lock_guard<Mutex> lock(*mtx);
  auto value = internal_queue->front();
  internal_queue->pop_front();
  return value;
}

// Used to avoid linker errors
// For more information, see: https://isocpp.org/wiki/faq/templates#class-templates
template class BlockingQueue<ConsoleThread::Message>;
template class BlockingQueue<DispatcherMiddleman<>::CallData>;
