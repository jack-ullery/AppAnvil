#ifndef SRC_THREADS_DISPATCHER_MIDDLEMAN_H
#define SRC_THREADS_DISPATCHER_MIDDLEMAN_H

#include <glibmm/dispatcher.h>
#include <memory>
#include <mutex>

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
    enum CallState {
      NONE,
      PROFILE,
      PROCESS,
      LOGS,
      PROFILES_TEXT
    };

    // Receive method (called from main thread)
    void handle_signal();

  private:
    std::shared_ptr<Dispatcher> dispatch;

    CallState state;
    std::string data1;
    std::string data2;

    std::shared_ptr<Profiles> prof;
    std::shared_ptr<Processes> proc;
    std::shared_ptr<Logs> logs;

    // Synchronization
    std::shared_ptr<Mutex> mtx;
};

#include "dispatcher_middleman.inl"

#endif // SRC_THREADS_DISPATCHER_MIDDLEMAN_H
