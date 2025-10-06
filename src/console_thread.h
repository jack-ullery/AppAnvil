#ifndef SRC_CONSOLE_THREAD_H
#define SRC_CONSOLE_THREAD_H

#include "threads/async_process.h"
#include "threads/blocking_queue.h"
#include "threads/dispatcher_middleman.h"

#include <chrono>
#include <condition_variable>
#include <functional>
#include <future>
#include <glibmm/dispatcher.h>
#include <mutex>
#include <vector>

/**
 * This class creates a separate thread that the main GUI thread can communicate with.
 * This second thread asynchronously calls terminal commands and communicates the results with the main thread.
 **/
class ConsoleThread
{
public:
  ConsoleThread(dispatch_cb_fun prof_cb, dispatch_cb_fun proc_cb, dispatch_cb_fun logs_cb, std::function<void(bool)> show_reauth);
  ~ConsoleThread();

  // Delete the copy-constructor, move constructor, and copy assignment operator
  ConsoleThread(const ConsoleThread &other)            = delete;
  ConsoleThread(const ConsoleThread &&other)           = delete;
  ConsoleThread &operator=(const ConsoleThread &other) = delete;

  // Create a move assignment operator
  ConsoleThread &operator=(ConsoleThread &&other) noexcept;

  void send_refresh_message();
  void send_change_profile_status_message(const std::string &profile, const std::string &old_status, const std::string &new_status);
  void send_quit_message();

protected:
  enum Event
  {
    REFRESH,
    CHANGE_STATUS,
    QUIT
  };

  struct Message
  {
    Event event;
    std::vector<std::string> data;

    Message(Event a, std::vector<std::string> b)
      : event{ a },
        data{ std::move(b) }
    {
    }
  };

  void console_caller();

private:
  static constexpr std::chrono::seconds TIME_WAIT = std::chrono::seconds(5);
  static std::chrono::time_point<std::chrono::steady_clock> get_wait_time_point();

  Message wait_for_message();
  void handle_refresh();

  // Member fields
  BlockingQueue<Message> queue;

  AsyncProcess aa_caller_proc;

  // DispatcherMiddleman used to communicate results with main thread
  DispatcherMiddleman<> dispatch_man;

  // Representation of the extra thread
  std::future<void> asynchronous_thread;

  // Synchronization Primitives
  std::mutex task_ready_mtx;
  std::condition_variable cv;
};

#endif // CONSOLE_THREAD_H
