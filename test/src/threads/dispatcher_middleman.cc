#include "../tabs/logs_status_mock.cc"
#include "../tabs/processes_status_mock.cc"
#include "../tabs/profiles_status_mock.cc"
#include "./dispatcher_middleman_mock.cc"
#include "./mutex_mock.cc"

#include <future>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::Sequence;

class DispatcherMiddlemanTest : public ::testing::Test
{
protected:
  DispatcherMiddlemanTest()
      : disp{new GlibDispatcherMock()}, prof{new ProfilesStatusMock()}, proc{new ProcessesStatusMock()}, logs{new LogsStatusMock()},
        mtx_mock{new MutexMock()}, dispatch_man(disp, prof, proc, logs, mtx_mock)
  {
  }

  virtual void SetUp() { }

  void expect_locks(unsigned int num);

  std::string profiles_arg  = "string argument for 'profiles.add_data()'";
  std::string processes_arg = "string argument for 'processes.add_data()'";
  std::string logs_arg      = "string argument for 'logs.add_data()'";

  std::shared_ptr<GlibDispatcherMock> disp;
  std::shared_ptr<ProfilesStatusMock> prof;
  std::shared_ptr<ProcessesStatusMock> proc;
  std::shared_ptr<LogsStatusMock> logs;
  std::shared_ptr<MutexMock> mtx_mock;

  DispatcherMiddlemanMock<ProfilesStatusMock, ProcessesStatusMock, LogsStatusMock, GlibDispatcherMock, MutexMock> dispatch_man;
};

// Expect the mutex to be locked and unlocked `num` times
// If lock/unlock is called in the wrong order this throws an error
// Fuerthermore, `mtx_mock` is a fake mutex which never blocks.
void DispatcherMiddlemanTest::expect_locks(unsigned int num)
{
  Sequence lock_calls;

  for(unsigned int i = 0; i < num; i++) {
    EXPECT_CALL(*mtx_mock, lock()).Times(1).InSequence(lock_calls);

    EXPECT_CALL(*mtx_mock, unlock()).Times(1).InSequence(lock_calls);
  }
}

TEST_F(DispatcherMiddlemanTest, UPDATE_PROFILES)
{
  Sequence main_calls;
  EXPECT_CALL(*disp, emit()).Times(1).InSequence(main_calls);
  EXPECT_CALL(*prof, add_data_to_record(profiles_arg)).Times(1).InSequence(main_calls);

  expect_locks(2);

  dispatch_man.update_profiles(profiles_arg);
  dispatch_man.handle_signal();
}

TEST_F(DispatcherMiddlemanTest, UPDATE_PROCESSES)
{
  Sequence main_calls;
  EXPECT_CALL(*disp, emit()).Times(1).InSequence(main_calls);
  EXPECT_CALL(*proc, add_data_to_record(processes_arg)).Times(1).InSequence(main_calls);

  expect_locks(2);

  dispatch_man.update_processes(processes_arg);
  dispatch_man.handle_signal();
}

TEST_F(DispatcherMiddlemanTest, UPDATE_LOGS)
{
  Sequence main_calls;
  EXPECT_CALL(*disp, emit()).Times(1).InSequence(main_calls);
  EXPECT_CALL(*logs, add_data_to_record(logs_arg)).Times(1).InSequence(main_calls);

  expect_locks(2);

  dispatch_man.update_logs(logs_arg);
  dispatch_man.handle_signal();
}

TEST_F(DispatcherMiddlemanTest, UPDATE_PROFILES_PROCESSES_SEQUENTIAL)
{
  EXPECT_CALL(*disp, emit()).Times(2);

  EXPECT_CALL(*prof, add_data_to_record(profiles_arg)).Times(1);
  EXPECT_CALL(*proc, add_data_to_record(processes_arg)).Times(1);

  expect_locks(4);

  dispatch_man.update_profiles(profiles_arg);
  dispatch_man.handle_signal();
  dispatch_man.update_processes(processes_arg);
  dispatch_man.handle_signal();
}

TEST_F(DispatcherMiddlemanTest, UPDATE_PROFILES_PROCESSES_INTERLOCKING)
{
  EXPECT_CALL(*disp, emit()).Times(2);

  EXPECT_CALL(*prof, add_data_to_record(profiles_arg)).Times(1);
  EXPECT_CALL(*proc, add_data_to_record(processes_arg)).Times(1);

  expect_locks(4);

  dispatch_man.update_profiles(profiles_arg);
  dispatch_man.update_processes(processes_arg);
  dispatch_man.handle_signal();
  dispatch_man.handle_signal();
}
