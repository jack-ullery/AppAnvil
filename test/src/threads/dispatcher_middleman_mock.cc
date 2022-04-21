#include "../../../src/threads/dispatcher_middleman.cc"
#include "../tabs/logs_status_mock.cc"
#include "../tabs/processes_status_mock.cc"
#include "../tabs/profiles_status_mock.cc"
#include "blocking_queue_mock.cc"
// #include "mutex_mock.cc"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

class GlibDispatcherMock
{
public:
  MOCK_METHOD(void, emit, ());
  MOCK_METHOD(void, connect, (const sigc::slot<void()> &slot));
};

template<class Profiles, class Processes, class Logs, class Dispatcher, class Mutex>
class DispatcherMiddlemanMock : public DispatcherMiddleman<Profiles, Processes, Logs, Dispatcher, Mutex>
{
public:
  FRIEND_TEST(DispatcherMiddlemanTest, UPDATE_PROFILES);
  FRIEND_TEST(DispatcherMiddlemanTest, UPDATE_PROCESSES);
  FRIEND_TEST(DispatcherMiddlemanTest, UPDATE_LOGS);
  FRIEND_TEST(DispatcherMiddlemanTest, UPDATE_PROFILES_PROCESSES_SEQUENTIAL);
  FRIEND_TEST(DispatcherMiddlemanTest, UPDATE_PROFILES_PROCESSES_INTERLOCKING);

  explicit DispatcherMiddlemanMock(std::shared_ptr<Dispatcher> disp, std::shared_ptr<Profiles> prof, std::shared_ptr<Processes> proc,
                                   std::shared_ptr<Logs> logs, std::shared_ptr<Mutex> my_mtx)
      : DispatcherMiddleman<Profiles, Processes, Logs, Dispatcher, Mutex>(disp, prof, proc, logs, my_mtx)
  {
  }
};

// Used to avoid linker errors
// For more information, see: https://isocpp.org/wiki/faq/templates#class-templates
template class DispatcherMiddleman<ProfilesStatusMock, ProcessesStatusMock, LogsStatusMock, GlibDispatcherMock, MutexMock>;
template class BlockingQueue<
    DispatcherMiddleman<ProfilesStatusMock, ProcessesStatusMock, LogsStatusMock, GlibDispatcherMock, MutexMock>::CallData,
    std::deque<DispatcherMiddleman<ProfilesStatusMock, ProcessesStatusMock, LogsStatusMock, GlibDispatcherMock, MutexMock>::CallData,
               std::allocator<
                   DispatcherMiddleman<ProfilesStatusMock, ProcessesStatusMock, LogsStatusMock, GlibDispatcherMock, MutexMock>::CallData>>,
    MutexMock>;
