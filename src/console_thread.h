#ifndef SRC_CONSOLE_THREAD_H
#define SRC_CONSOLE_THREAD_H

#include "tabs/logs.h"
#include "tabs/processes.h"
#include "tabs/profiles.h"
#include "tabs/file_chooser.h"
#include "threads/blocking_queue.h"
#include "threads/dispatcher_middleman.h"

#include <condition_variable>
#include <memory>
#include <mutex>
#include <future>
#include <thread>

enum TabState {
  PROFILE,
  PROCESS,
  LOGS,
  FILECHOOSER
};

enum Event {
  REFRESH,
  QUIT
};

class ConsoleThread
{
  public:
    ConsoleThread(std::shared_ptr<Profiles> prof, std::shared_ptr<Processes> proc, std::shared_ptr<Logs> logs, std::shared_ptr<FileChooser> file_chooser);
    ~ConsoleThread();

    void set_state(TabState new_state);
    void send_refresh_message();
    void send_quit_message();

    void get_status();
    void get_unconfined();
    void get_logs();

  protected:
    void console_caller();

  private:
    Event wait_for_message();
    void run_command();

    // Representation of the extra thread
    std::future<void> asynchronous_thread;

    // Member fields
    BlockingQueue<Event> queue;
    TabState current_state;

    // DispatcherMiddleman used to communicate results with main thread
    DispatcherMiddleman dispatch_man;

    // Synchronization Primitives
    std::mutex task_ready_mtx;
    std::mutex state_mtx;
    std::condition_variable cv;
};

#endif // CONSOLE_THREAD_H
