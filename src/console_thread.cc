#include "console_thread.h"
#include "tabs/controller/logs_controller.h"
#include "tabs/controller/processes_controller.h"
#include "tabs/controller/profiles_controller.h"
#include "tabs/model/database.h"
#include "tabs/model/status_column_record.h"
#include "tabs/view/logs.h"
#include "tabs/view/processes.h"
#include "tabs/view/profiles.h"
#include "threads/command_caller.h"

#include <memory>
#include <string>

template<class ProfilesController, class ProcessesController, class LogsController>
ConsoleThread<ProfilesController, ProcessesController, LogsController>::ConsoleThread(std::shared_ptr<ProfilesController> prof,
                                                                                      std::shared_ptr<ProcessesController> proc,
                                                                                      std::shared_ptr<LogsController> logs)
  : aa_caller_proc{ CommandCaller::call_aa_caller() },
    dispatch_man(std::move(prof), std::move(proc), std::move(logs)),
    asynchronous_thread(
      std::async(std::launch::async, &ConsoleThread<ProfilesController, ProcessesController, LogsController>::console_caller, this))
{
}

template<class ProfilesController, class ProcessesController, class LogsController>
void ConsoleThread<ProfilesController, ProcessesController, LogsController>::send_refresh_message()
{
  std::unique_lock<std::mutex> lock(task_ready_mtx);
  // Create a message with the state to refresh for, but no data
  Message message(REFRESH, {});
  // Send the message to the queue, this lets the other thread know what it should do.
  queue.push(message);
  cv.notify_one();
}

template<class ProfilesController, class ProcessesController, class LogsController>
void ConsoleThread<ProfilesController, ProcessesController, LogsController>::send_change_profile_status_message(
  const std::string &profile,
  const std::string &old_status,
  const std::string &new_status)
{
  std::unique_lock<std::mutex> lock(task_ready_mtx);
  // Create a message with the state to refresh for, but no data
  Message message(CHANGE_STATUS, { profile, old_status, new_status });
  // Send the message to the queue, this lets the other thread know what it should do.
  queue.push(message);
  cv.notify_one();
}

template<class ProfilesController, class ProcessesController, class LogsController>
void ConsoleThread<ProfilesController, ProcessesController, LogsController>::send_quit_message()
{
  std::unique_lock<std::mutex> lock(task_ready_mtx);
  Message message(QUIT, {});
  queue.push(message);
  cv.notify_one();
}

template<class ProfilesController, class ProcessesController, class LogsController>
void ConsoleThread<ProfilesController, ProcessesController, LogsController>::reenable_authentication_for_refresh()
{
  std::unique_lock<std::mutex> lock(task_ready_mtx);
  should_try_refresh = true;
  lock.unlock();

  // Send a refresh message
  send_refresh_message();
}

template<class ProfilesController, class ProcessesController, class LogsController>
std::chrono::time_point<std::chrono::steady_clock>
ConsoleThread<ProfilesController, ProcessesController, LogsController>::get_wait_time_point()
{
  auto now       = std::chrono::steady_clock::now();
  auto time_wait = std::chrono::seconds(ConsoleThread<ProfilesController, ProcessesController, LogsController>::TIME_WAIT);
  return now + time_wait;
}

template<class ProfilesController, class ProcessesController, class LogsController>
typename ConsoleThread<ProfilesController, ProcessesController, LogsController>::Message
ConsoleThread<ProfilesController, ProcessesController, LogsController>::wait_for_message()
{
  std::unique_lock<std::mutex> lock(task_ready_mtx);

  while (queue.empty()) {
    auto cv_status = cv.condition_variable::wait_until(lock, get_wait_time_point()); // Look into `wait_until`

    if (cv_status == std::cv_status::timeout) {
      // Create a message with the state to refresh for, but no data
      Message message(REFRESH, {});
      // Send the message to the queue, this lets the other thread know what it should do.
      queue.push(message);
    }
  }

  return queue.pop();
}

template<class ProfilesController, class ProcessesController, class LogsController>
void ConsoleThread<ProfilesController, ProcessesController, LogsController>::handle_refresh()
{
  
}

template<class ProfilesController, class ProcessesController, class LogsController>
void ConsoleThread<ProfilesController, ProcessesController, LogsController>::console_caller()
{
  bool shouldContinue = true;
  while (shouldContinue) {
    Message message = wait_for_message();

    switch (message.event) {
      case REFRESH:
        handle_refresh();
        break;

      case CHANGE_STATUS: {
        const std::string profile    = message.data.at(0);
        const std::string old_status = message.data.at(1);
        const std::string new_status = message.data.at(2);
        std::string return_message   = CommandCaller::execute_change(profile, old_status, new_status);
        dispatch_man.update_prof_apply_text(return_message);
        handle_refresh();
      } break;

      case QUIT:
        shouldContinue = false;
        break;
    }
  }
}

// Move Assignment Operator
template<class ProfilesController, class ProcessesController, class LogsController>
ConsoleThread<ProfilesController, ProcessesController, LogsController> &
ConsoleThread<ProfilesController, ProcessesController, LogsController>::operator=(ConsoleThread &&other) noexcept
{
  queue = other.queue;
  // aa_caller_proc = other.aa_caller_proc;
  // dispatch_man = other.dispatch_man;
  return *this;
}

template<class ProfilesController, class ProcessesController, class LogsController>
ConsoleThread<ProfilesController, ProcessesController, LogsController>::~ConsoleThread()
{
  send_quit_message();
  asynchronous_thread.wait();
}

template class ConsoleThread<ProfilesController<Profiles, Database, ProfileAdapter<Database>>,
                             ProcessesController<Processes, Database, ProcessAdapter<Database, StatusColumnRecord>>,
                             LogsController<Logs, Database, LogAdapter<Database, StatusColumnRecord>, LogRecord>>;