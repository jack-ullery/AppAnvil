#include "../tabs/controller/logs_controller_mock.h"
#include "../tabs/controller/processes_controller_mock.h"
#include "../tabs/controller/profiles_controller_mock.h"
#include "dispatcher_middleman.h"
#include "mutex_mock.h"

#include <future>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::Sequence;

// Expect the mutex to be locked and unlocked `num` times
// If lock/unlock is called in the wrong order this throws an error
// Fuerthermore, `mtx_mock` is a fake mutex which never blocks.
void
DispatcherMiddlemanTest::expect_locks(unsigned int num)
{
  Sequence lock_calls;

  for (unsigned int i = 0; i < num; i++) {
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
