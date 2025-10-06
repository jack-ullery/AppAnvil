#include "console_thread.h"
#include "threads/command_caller.h"

#include <iostream>
#include <json/reader.h>
#include <json/value.h>
#include <json/writer.h>
#include <string>

ConsoleThread::ConsoleThread(dispatch_cb_fun prof, dispatch_cb_fun proc, dispatch_cb_fun logs, std::function<void(bool)> show_reauth)
  : aa_caller_proc{ CommandCaller::call_aa_caller() },
    dispatch_man(prof, proc, logs, show_reauth),
    asynchronous_thread(std::async(std::launch::async, &ConsoleThread::console_caller, this))
{
}

void ConsoleThread::send_refresh_message()
{
  std::unique_lock<std::mutex> lock(task_ready_mtx);
  // Create a message with the state to refresh for, but no data
  Message message(REFRESH, {});
  // Send the message to the queue, this lets the other thread know what it should do.
  queue.push(message);
  cv.notify_one();
}

void ConsoleThread::send_change_profile_status_message(const std::string &profile,
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

void ConsoleThread::send_quit_message()
{
  std::unique_lock<std::mutex> lock(task_ready_mtx);
  Message message(QUIT, {});
  queue.push(message);
  cv.notify_one();
}

typename ConsoleThread::Message ConsoleThread::wait_for_message()
{
  std::unique_lock<std::mutex> lock(task_ready_mtx);

  while (queue.empty()) {
    auto cv_status = cv.condition_variable::wait_for(lock, ConsoleThread::TIME_WAIT); // Look into `wait_until`

    if (cv_status == std::cv_status::timeout) {
      // Create a message with the state to refresh for, but no data
      Message message(REFRESH, {});
      // Send the message to the queue, this lets the other thread know what it should do.
      queue.push(message);
    }
  }

  return queue.pop();
}

void ConsoleThread::handle_refresh()
{
  dispatch_man.update_reauth(!aa_caller_proc.valid());
  if (!aa_caller_proc.valid()) {
    return;
  }

  Json::Reader reader;
  for (const std::string &line : aa_caller_proc.readlines()) {
    Json::Value root;
    reader.parse(line, root);
    for (const std::string &key : root.getMemberNames()) {
      Json::Value &value = root[key];
      if (key == "status") {
        dispatch_man.update_profiles(value);
      } else if (key == "ps") {
        dispatch_man.update_processes(value);
      } else if (key == "journalctl") {
        dispatch_man.update_logs(value);
      } else {
        std::cerr << "Unkown key from aa-caller: " << key << std::endl;
      }
      // std::cerr << "done: " << key << std::endl;
    }
  }
}

void ConsoleThread::console_caller()
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
ConsoleThread &ConsoleThread::operator=(ConsoleThread &&other) noexcept
{
  queue = other.queue;
  // aa_caller_proc = other.aa_caller_proc;
  // dispatch_man = other.dispatch_man;
  return *this;
}

ConsoleThread::~ConsoleThread()
{
  send_quit_message();
  asynchronous_thread.wait();
}
