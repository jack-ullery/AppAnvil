#ifndef SRC_THREADS_BLOCKING_QUEUE_H
#define SRC_THREADS_BLOCKING_QUEUE_H

#include <deque>
#include <mutex>

template <class T>
class BlockingQueue
{
  public:
    // Constructor
    BlockingQueue();

    // Accessor Methods
    T front();
    T back();
    int size();
    bool empty();
    
    // Mutator Methods
    void clear();
    void push(const T& value);
    void push_front(const T& value);
    T pop();

  private:
    // The internal queue that this class acts as a wrapper for
    std::deque<T> internal_queue;
    // A mutex to lock the queue when performing operations
    std::mutex mtx;
};

/**
 * Important Note: Because of how the C++ compiler links code with templates, we need to have implementation
 * in the header file
 **/

template <class T>
BlockingQueue<T>::BlockingQueue() = default;

// Accessor Methods
template <class T>
T BlockingQueue<T>::front(){
  std::lock_guard<std::mutex> lock(mtx);
  return internal_queue.front();
}

template <class T>
T BlockingQueue<T>::back(){
  std::lock_guard<std::mutex> lock(mtx);
  return internal_queue.back();
}

template <class T>
int BlockingQueue<T>::size(){
  std::lock_guard<std::mutex> lock(mtx);
  return internal_queue.size();
}

template <class T>
bool BlockingQueue<T>::empty(){
  std::lock_guard<std::mutex> lock(mtx);
  return internal_queue.empty();
}

// Mutator Methods
template <class T>
void BlockingQueue<T>::clear(){
  std::lock_guard<std::mutex> lock(mtx);
  internal_queue.clear();
}

template <class T>
void BlockingQueue<T>::push(const T& value){
  std::lock_guard<std::mutex> lock(mtx);
  internal_queue.push_back(value);
}

template <class T>
void BlockingQueue<T>::push_front(const T& value){
  std::lock_guard<std::mutex> lock(mtx);
  internal_queue.push_front(value);
}

template <class T>
T BlockingQueue<T>::pop(){
  std::lock_guard<std::mutex> lock(mtx);
  auto value = internal_queue.front();
  internal_queue.pop_front();
  return value;
}


#endif // SRC_THREADS_BLOCKING_QUEUE_H
