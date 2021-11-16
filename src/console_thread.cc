#include "console_thread.h"
#include "threads/command_caller.h"

#include <iostream>
#include <tuple>

ConsoleThread::ConsoleThread(std::shared_ptr<Profiles> prof, std::shared_ptr<Processes> proc, std::shared_ptr<Logs> logs)
: dispatch_man(std::move(prof), std::move(proc), std::move(logs))
{
  std::lock_guard<std::mutex> lock(state_mtx);
  current_state = PROFILE;
  asynchronous_thread = std::async(std::launch::async, &ConsoleThread::console_caller, this);;
}

void ConsoleThread::set_state(TabState new_state){
  std::lock_guard<std::mutex> lock(state_mtx);
  current_state = new_state;
}

void ConsoleThread::send_refresh_message(){
  std::unique_lock<std::mutex> lock(task_ready_mtx);
  queue.push(REFRESH);
  cv.notify_one();
}

void ConsoleThread::send_quit_message(){
  std::unique_lock<std::mutex> lock(task_ready_mtx);
  queue.push(QUIT);
  cv.notify_one();
}

void ConsoleThread::run_command(){
  std::lock_guard<std::mutex> lock(state_mtx);
  switch (current_state)
  {
    case PROFILE:
    {
      std::string status = CommandCaller::get_status_str();
      dispatch_man.update_profiles(status);
    }
    break;

    case PROCESS:
    {
      std::string status = CommandCaller::get_status_str();
      std::string unconf = CommandCaller::get_unconfined();
      dispatch_man.update_processes(status, unconf);
    }
    break;

    case LOGS:
    {
      std::string logs = CommandCaller::get_logs_str();
      dispatch_man.update_logs(logs);
    }
    break;
  }
}

Event ConsoleThread::wait_for_message(){
  std::unique_lock<std::mutex> lock(task_ready_mtx);
  while(queue.empty()){
    cv.condition_variable::wait(lock); // Look into `wait_until`
  }

  return queue.pop();
}

void ConsoleThread::console_caller(){
  bool shouldContinue = true;
  while(shouldContinue) {
    auto message = wait_for_message();
    switch (message)
    {
      case REFRESH:
        run_command();
        break;
      
      case QUIT:
        shouldContinue = false;
        break;
    }
  }
}

ConsoleThread& ConsoleThread::operator=(ConsoleThread&& other) noexcept{
  std::ignore = other;
  return *this;
}

ConsoleThread::~ConsoleThread(){
  send_quit_message();
  asynchronous_thread.wait();
}
