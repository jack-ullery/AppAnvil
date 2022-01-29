template <class T, class Mutex>
BlockingQueue<T, Mutex>::BlockingQueue()
: mtx{new Mutex()}
{ }

template <class T, class Mutex>
BlockingQueue<T, Mutex>::BlockingQueue(std::deque<T> queue, std::shared_ptr<Mutex> my_mtx)
: internal_queue{std::move(queue)},
  mtx{std::move(my_mtx)}
{ }

// Accessor Methods
template <class T, class Mutex>
T BlockingQueue<T, Mutex>::front(){
  std::lock_guard<Mutex> lock(*mtx);
  return internal_queue.front();
}

template <class T, class Mutex>
T BlockingQueue<T, Mutex>::back(){
  std::lock_guard<Mutex> lock(*mtx);
  return internal_queue.back();
}

template <class T, class Mutex>
int BlockingQueue<T, Mutex>::size(){
  std::lock_guard<Mutex> lock(*mtx);
  return internal_queue.size();
}

template <class T, class Mutex>
bool BlockingQueue<T, Mutex>::empty(){
  std::lock_guard<Mutex> lock(*mtx);
  return internal_queue.empty();
}

// Mutator Methods
template <class T, class Mutex>
void BlockingQueue<T, Mutex>::clear(){
  std::lock_guard<Mutex> lock(*mtx);
  internal_queue.clear();
}

template <class T, class Mutex>
void BlockingQueue<T, Mutex>::push(const T& value){
  std::lock_guard<Mutex> lock(*mtx);
  internal_queue.push_back(value);
}

template <class T, class Mutex>
void BlockingQueue<T, Mutex>::push_front(const T& value){
  std::lock_guard<Mutex> lock(*mtx);
  internal_queue.push_front(value);
}

template <class T, class Mutex>
T BlockingQueue<T, Mutex>::pop(){
  std::lock_guard<Mutex> lock(*mtx);
  auto value = internal_queue.front();
  internal_queue.pop_front();
  return value;
}
