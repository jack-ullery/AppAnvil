#ifndef SRC_THREADS_DISPATCHER_MIDDLEMAN_H
#define SRC_THREADS_DISPATCHER_MIDDLEMAN_H

#include "blocking_queue.h"

#include <glibmm/dispatcher.h>
#include <memory>

/**
 * Class to extend some of the functionality of `Dispatcher` to easier facilitate inter-thread
 * communication between the second thread and main thread. This is needed to prevent some
 * concurrency errors with the GUI.
 **/
template <class Profiles, class Processes, class Logs, class Dispatcher, class Mutex>
class DispatcherMiddleman
{
public:
  // Constructor
  DispatcherMiddleman(std::shared_ptr<Profiles> prof, std::shared_ptr<Processes> proc, std::shared_ptr<Logs> logs);
  // For unit testing
  explicit DispatcherMiddleman(std::shared_ptr<Dispatcher> disp, std::shared_ptr<Profiles> prof, std::shared_ptr<Processes> proc, std::shared_ptr<Logs> logs,
                               std::shared_ptr<Mutex> my_mtx);

  // Send methods (called from second thread)
  void update_profiles(const std::string& confined);
  void update_processes(const std::string& confined, const std::string& unconfined);
  void update_logs(const std::string& logs);
  void update_prof_apply_text(const std::string& text);

protected:
  enum CallType {
    NONE,
    PROFILE,
    PROCESS,
    LOGS,
    PROFILES_TEXT
  };

  struct CallData {
    CallType type;
    std::string arg_1;
    std::string arg_2;

    CallData(CallType a, const std::string& b)
    {
      type = a;
      arg_1 = b;
      arg_2 = "";
    }

    CallData(CallType a, const std::string& b, const std::string& c)
    {
      if(a != PROCESS) {
        throw std::invalid_argument("Two argument constructor used, when only one argument expected. `Process` is the only known instance when two arguments are used.");
      }

      this->type = std::move(a);
      this->arg_1 = b;
      this->arg_2 = c;
    }
  };

  // Receive method (called from main thread)
  void handle_signal();

private:
  BlockingQueue<CallData, Mutex> queue;
  std::shared_ptr<Dispatcher> dispatch;

  std::shared_ptr<Profiles> prof;
  std::shared_ptr<Processes> proc;
  std::shared_ptr<Logs> logs;
};

/**
 * Important Note: Because of how the C++ compiler links code with templates, we need to have implementation
 * included in the header file
 **/
#include "dispatcher_middleman.inl"

#endif // SRC_THREADS_DISPATCHER_MIDDLEMAN_H
