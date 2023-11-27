#ifndef TEST_SRC_THREADS_DISPATCHER_MIDDLEMAN_H
#define TEST_SRC_THREADS_DISPATCHER_MIDDLEMAN_H

#include "../../../src/threads/dispatcher_middleman.cc"
#include "../tabs/controller/logs_controller_mock.h"
#include "../tabs/controller/processes_controller_mock.h"
#include "../tabs/controller/profiles_controller_mock.h"
#include "blocking_queue_mock.h"
#include "mutex_mock.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

class GlibDispatcherMock
{
public:
  MOCK_METHOD(void, emit, ());
  MOCK_METHOD(void, connect, (const sigc::slot<void()> &slot));
};

class DispatcherMiddlemanTest : public ::testing::Test
{
protected:
  DispatcherMiddlemanTest()
    : disp{ std::make_shared<GlibDispatcherMock>() },
      prof{ std::make_shared<ProfilesControllerMock>() },
      proc{ std::make_shared<ProcessesControllerMock>() },
      logs{ std::make_shared<LogsControllerMock>() },
      mtx_mock{ std::make_shared<MutexMock>() },
      dispatch_man(disp, prof, proc, logs, mtx_mock)
  {
  }

  virtual void SetUp() {}

  void expect_locks(unsigned int num);
  void expect_show_reauthenticate_prompt(bool should_show, int times);

  std::string profiles_arg  = "string argument for 'profiles.add_data()'";
  std::string processes_arg = "string argument for 'processes.add_data()'";
  std::list<std::shared_ptr<LogRecord>> logs_arg;

  std::shared_ptr<GlibDispatcherMock> disp;
  std::shared_ptr<ProfilesControllerMock> prof;
  std::shared_ptr<ProcessesControllerMock> proc;
  std::shared_ptr<LogsControllerMock> logs;
  std::shared_ptr<MutexMock> mtx_mock;

  DispatcherMiddleman<ProfilesControllerMock, ProcessesControllerMock, LogsControllerMock, GlibDispatcherMock, MutexMock> dispatch_man;
};

// Used to avoid linker errors
// For more information, see: https://isocpp.org/wiki/faq/templates#class-templates
template class DispatcherMiddleman<ProfilesControllerMock, ProcessesControllerMock, LogsControllerMock, GlibDispatcherMock, MutexMock>;
template class BlockingQueue<
  DispatcherMiddleman<ProfilesControllerMock, ProcessesControllerMock, LogsControllerMock, GlibDispatcherMock, MutexMock>::CallData,
  std::deque<DispatcherMiddleman<ProfilesControllerMock, ProcessesControllerMock, LogsControllerMock, GlibDispatcherMock, MutexMock>::CallData,
             std::allocator<
               DispatcherMiddleman<ProfilesControllerMock, ProcessesControllerMock, LogsControllerMock, GlibDispatcherMock, MutexMock>::CallData>>,
  MutexMock>;

#endif