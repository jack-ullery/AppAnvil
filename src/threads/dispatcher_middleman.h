#ifndef SRC_THREADS_DISPATCHER_MIDDLEMAN_H
#define SRC_THREADS_DISPATCHER_MIDDLEMAN_H

#include "blocking_queue.h"

#include <glibmm/dispatcher.h>
#include <json/value.h>
#include <memory>

// If the unit tests are enabled, include the following header
#ifdef TESTS_ENABLED
#include <gtest/gtest.h>
#endif

typedef std::function<void(std::string &)> dispatch_cb_fun;

/**
 * Class to extend some of the functionality of `Dispatcher` to easier facilitate inter-thread
 * communication between the second thread and main thread. This is needed to prevent some
 * concurrency errors with the GUI.
 **/
template<class Dispatcher = Glib::Dispatcher, class Mutex = std::mutex>
class DispatcherMiddleman
{
public:
  // Constructor
  DispatcherMiddleman(dispatch_cb_fun prof, dispatch_cb_fun proc, dispatch_cb_fun logs, std::function<void(bool)> show_reauth);
  // For unit testing
  explicit DispatcherMiddleman(std::shared_ptr<Dispatcher> disp,
                               dispatch_cb_fun prof,
                               dispatch_cb_fun proc,
                               dispatch_cb_fun logs,
                               std::shared_ptr<Mutex> my_mtx);

  // Send methods (called from second thread)
  void update_profiles(Json::Value &value);
  void update_processes(Json::Value &value);
  void update_logs(Json::Value &value);
  void update_reauth(const bool &reauth);
  void update_prof_apply_text(const std::string &text);

protected:
  enum CallType
  {
    NONE,
    PROFILE,
    PROCESS,
    LOGS,
    PROFILES_TEXT,
    REAUTH
  };

  struct CallData
  {
    CallType type;
    std::string data;
    bool should_reauth;

    CallData(CallType type, Json::Value data)
      : type{ type },
        data{ data.asString() }
    {
    }

    CallData(bool should_reauth)
      : type{ REAUTH },
        should_reauth{ should_reauth }
    {
    }
  };

  // Receive method (called from main thread)
  void handle_signal();

private:
  BlockingQueue<CallData, Mutex> queue;
  std::shared_ptr<Dispatcher> dispatch;

  dispatch_cb_fun prof;
  dispatch_cb_fun proc;
  dispatch_cb_fun logs;
  std::function<void(bool)> show_reauth;

#ifdef TESTS_ENABLED
  FRIEND_TEST(DispatcherMiddlemanTest, UPDATE_PROFILES);
  FRIEND_TEST(DispatcherMiddlemanTest, UPDATE_PROCESSES);
  FRIEND_TEST(DispatcherMiddlemanTest, UPDATE_LOGS);
  FRIEND_TEST(DispatcherMiddlemanTest, UPDATE_PROFILES_PROCESSES_SEQUENTIAL);
  FRIEND_TEST(DispatcherMiddlemanTest, UPDATE_PROFILES_PROCESSES_INTERLOCKING);
#endif
};

#endif // SRC_THREADS_DISPATCHER_MIDDLEMAN_H
