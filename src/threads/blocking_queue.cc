#include "blocking_queue.h"

#include "../console_thread.h"
#include "../tabs/controller/logs_controller.h"
#include "../tabs/controller/processes_controller.h"
#include "../tabs/controller/profiles_controller.h"
#include "../tabs/model/database.h"
#include "../tabs/model/status_column_record.h"
#include "../tabs/view/logs.h"
#include "../tabs/view/processes.h"
#include "../tabs/view/profiles.h"

template<class T, class Deque, class Mutex>
BlockingQueue<T, Deque, Mutex>::BlockingQueue() : internal_queue{std::make_shared<Deque>()}, mtx{new Mutex()}
{
}

template<class T, class Deque, class Mutex>
BlockingQueue<T, Deque, Mutex>::BlockingQueue(std::shared_ptr<Deque> queue, std::shared_ptr<Mutex> my_mtx)
    : internal_queue{std::move(queue)}, mtx{std::move(my_mtx)}
{
}

// Accessor Methods
template<class T, class Deque, class Mutex> T BlockingQueue<T, Deque, Mutex>::front()
{
  std::lock_guard<Mutex> lock(*mtx);
  return internal_queue->front();
}

template<class T, class Deque, class Mutex> T BlockingQueue<T, Deque, Mutex>::back()
{
  std::lock_guard<Mutex> lock(*mtx);
  return internal_queue->back();
}

template<class T, class Deque, class Mutex> int BlockingQueue<T, Deque, Mutex>::size()
{
  std::lock_guard<Mutex> lock(*mtx);
  return internal_queue->size();
}

template<class T, class Deque, class Mutex> bool BlockingQueue<T, Deque, Mutex>::empty()
{
  std::lock_guard<Mutex> lock(*mtx);
  return internal_queue->empty();
}

// Mutator Methods
template<class T, class Deque, class Mutex> void BlockingQueue<T, Deque, Mutex>::clear()
{
  std::lock_guard<Mutex> lock(*mtx);
  internal_queue->clear();
}

template<class T, class Deque, class Mutex> void BlockingQueue<T, Deque, Mutex>::push(const T &value)
{
  std::lock_guard<Mutex> lock(*mtx);
  internal_queue->push_back(value);
}

template<class T, class Deque, class Mutex> void BlockingQueue<T, Deque, Mutex>::push_front(const T &value)
{
  std::lock_guard<Mutex> lock(*mtx);
  internal_queue->push_front(value);
}

template<class T, class Deque, class Mutex> T BlockingQueue<T, Deque, Mutex>::pop()
{
  std::lock_guard<Mutex> lock(*mtx);
  auto value = internal_queue->front();
  internal_queue->pop_front();
  return value;
}

// Used to avoid linker errors
// For more information, see: https://isocpp.org/wiki/faq/templates#class-templates
// These next few lines are very gross, as I copied them directly from a Linker error. Don't even try to read them.
template class BlockingQueue<ConsoleThread<ProfilesController<Profiles, StatusColumnRecord, Database>, ProcessesController<Processes, StatusColumnRecord>, LogsController<Logs, StatusColumnRecord> >::Message, std::deque<ConsoleThread<ProfilesController<Profiles, StatusColumnRecord, Database>, ProcessesController<Processes, StatusColumnRecord>, LogsController<Logs, StatusColumnRecord> >::Message, std::allocator<ConsoleThread<ProfilesController<Profiles, StatusColumnRecord, Database>, ProcessesController<Processes, StatusColumnRecord>, LogsController<Logs, StatusColumnRecord> >::Message>>, std::mutex>;
template class BlockingQueue<DispatcherMiddleman<ProfilesController<Profiles, StatusColumnRecord, Database>, ProcessesController<Processes, StatusColumnRecord>, LogsController<Logs, StatusColumnRecord>, Glib::Dispatcher, std::mutex>::CallData, std::deque<DispatcherMiddleman<ProfilesController<Profiles, StatusColumnRecord, Database>, ProcessesController<Processes, StatusColumnRecord>, LogsController<Logs, StatusColumnRecord>, Glib::Dispatcher, std::mutex>::CallData, std::allocator<DispatcherMiddleman<ProfilesController<Profiles, StatusColumnRecord, Database>, ProcessesController<Processes, StatusColumnRecord>, LogsController<Logs, StatusColumnRecord>, Glib::Dispatcher, std::mutex>::CallData> >, std::mutex>;