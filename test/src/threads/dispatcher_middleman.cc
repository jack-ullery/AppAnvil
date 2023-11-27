#include "dispatcher_middleman.h"
#include "../tabs/controller/logs_controller_mock.h"
#include "../tabs/controller/processes_controller_mock.h"
#include "../tabs/controller/profiles_controller_mock.h"
#include "mutex_mock.h"

#include <future>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::Sequence;

// Expect the mutex to be locked and unlocked `num` times
// If lock/unlock is called in the wrong order this throws an error
// Fuerthermore, `mtx_mock` is a fake mutex which never blocks.
void DispatcherMiddlemanTest::expect_locks(unsigned int num)
{
  Sequence lock_calls;

  for (unsigned int i = 0; i < num; i++) {
    EXPECT_CALL(*mtx_mock, lock()).Times(1).InSequence(lock_calls);
    EXPECT_CALL(*mtx_mock, unlock()).Times(1).InSequence(lock_calls);
  }
}

void DispatcherMiddlemanTest::expect_show_reauthenticate_prompt(bool should_show, int times)
{
  EXPECT_CALL(*prof->get_tab(), show_reauthenticate_prompt_impl(should_show)).Times(times);
  EXPECT_CALL(*proc->get_tab(), show_reauthenticate_prompt_impl(should_show)).Times(times);
  EXPECT_CALL(*logs->get_tab(), show_reauthenticate_prompt_impl(should_show)).Times(times);
}

TEST_F(DispatcherMiddlemanTest, UPDATE_PROFILES)
{
  bool should_show = false;

  Sequence main_calls;
  EXPECT_CALL(*disp, emit()).Times(1).InSequence(main_calls);
  EXPECT_CALL(*prof, add_data_to_record(profiles_arg)).Times(1).InSequence(main_calls);

  int num_calls = 1;
  expect_locks(2 * num_calls);
  expect_show_reauthenticate_prompt(should_show, num_calls);

  dispatch_man.update_profiles(profiles_arg, should_show);
  dispatch_man.handle_signal();
}

TEST_F(DispatcherMiddlemanTest, UPDATE_PROCESSES)
{
  bool should_show = false;

  Sequence main_calls;
  EXPECT_CALL(*disp, emit()).Times(1).InSequence(main_calls);
  EXPECT_CALL(*proc, add_data_to_record(processes_arg)).Times(1).InSequence(main_calls);

  int num_calls = 1;
  expect_locks(2 * num_calls);
  expect_show_reauthenticate_prompt(should_show, num_calls);

  dispatch_man.update_processes(processes_arg, should_show);;
  dispatch_man.handle_signal();
}

TEST_F(DispatcherMiddlemanTest, UPDATE_LOGS)
{
  bool should_show = false;

  Sequence main_calls;
  EXPECT_CALL(*disp, emit()).Times(1).InSequence(main_calls);
  EXPECT_CALL(*logs, add_data_to_record(logs_arg)).Times(1).InSequence(main_calls);

  int num_calls = 1;
  expect_locks(2 * num_calls);
  expect_show_reauthenticate_prompt(should_show, num_calls);

  dispatch_man.update_logs(logs_arg, should_show);;
  dispatch_man.handle_signal();
}

TEST_F(DispatcherMiddlemanTest, UPDATE_PROFILES_PROCESSES_SEQUENTIAL)
{
  bool should_show = false;

  EXPECT_CALL(*disp, emit()).Times(2);

  EXPECT_CALL(*prof, add_data_to_record(profiles_arg)).Times(1);
  EXPECT_CALL(*proc, add_data_to_record(processes_arg)).Times(1);

  int num_calls = 2;
  expect_locks(2 * num_calls);
  expect_show_reauthenticate_prompt(should_show, num_calls);

  dispatch_man.update_profiles(profiles_arg, should_show);;
  dispatch_man.handle_signal();
  dispatch_man.update_processes(processes_arg, should_show);;
  dispatch_man.handle_signal();
}

TEST_F(DispatcherMiddlemanTest, UPDATE_PROFILES_PROCESSES_INTERLOCKING)
{
  bool should_show = false;

  EXPECT_CALL(*disp, emit()).Times(2);

  EXPECT_CALL(*prof, add_data_to_record(profiles_arg)).Times(1);
  EXPECT_CALL(*proc, add_data_to_record(processes_arg)).Times(1);

  int num_calls = 2;
  expect_locks(2 * num_calls);
  expect_show_reauthenticate_prompt(should_show, num_calls);

  dispatch_man.update_profiles(profiles_arg, should_show);;
  dispatch_man.update_processes(processes_arg, should_show);;
  dispatch_man.handle_signal();
  dispatch_man.handle_signal();
}
