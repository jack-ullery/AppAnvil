#include "../../../src/threads/dispatcher_middleman.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

class GlibDispatcherMock
{
public:
  MOCK_METHOD(void, emit, ());
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
