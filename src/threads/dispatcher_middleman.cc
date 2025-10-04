#include "dispatcher_middleman.h"

#include <glibmm/priorities.h>
#include <iostream>
#include <sigc++/functors/mem_fun.h>

template<class Dispatcher, class Mutex>
DispatcherMiddleman<Dispatcher, Mutex>::DispatcherMiddleman(dispatch_cb_fun prof_arg,
                                                            dispatch_cb_fun proc_arg,
                                                            dispatch_cb_fun logs_arg,
                                                            std::function<void(bool)> show_reauth)
  : dispatch{ std::make_shared<Dispatcher>() },
    prof{ std::move(prof_arg) },
    proc{ std::move(proc_arg) },
    logs{ std::move(logs_arg) },
    show_reauth{ std::move(show_reauth) }
{
  auto function = sigc::mem_fun(*this, &DispatcherMiddleman<Dispatcher, Mutex>::handle_signal);
  dispatch->connect(function);
}

template<class Dispatcher, class Mutex>
DispatcherMiddleman<Dispatcher, Mutex>::DispatcherMiddleman(std::shared_ptr<Dispatcher> disp,
                                                            dispatch_cb_fun prof_arg,
                                                            dispatch_cb_fun proc_arg,
                                                            dispatch_cb_fun logs_arg,
                                                            std::shared_ptr<Mutex> my_mtx)
  : queue(std::make_shared<std::deque<CallData>>(), my_mtx),
    dispatch{ std::move(disp) },
    prof{ std::move(prof_arg) },
    proc{ std::move(proc_arg) },
    logs{ std::move(logs_arg) }
{
}

// Send methods (called from second thread)
template<class Dispatcher, class Mutex>
void DispatcherMiddleman<Dispatcher, Mutex>::update_profiles(const std::string &confined, const bool &had_authentication_error)
{
  CallData data(PROFILE, confined, had_authentication_error);
  queue.push(data);
  dispatch->emit();
}

template<class Dispatcher, class Mutex>
void DispatcherMiddleman<Dispatcher, Mutex>::update_processes(const std::string &unconfined, const bool &had_authentication_error)
{
  CallData data(PROCESS, unconfined, had_authentication_error);
  queue.push(data);
  dispatch->emit();
}

template<class Dispatcher, class Mutex>
void DispatcherMiddleman<Dispatcher, Mutex>::update_logs(const std::list<std::shared_ptr<LogRecord>> &logs,
                                                         const bool &had_authentication_error)
{
  CallData data(LOGS, logs, had_authentication_error);
  queue.push(data);
  dispatch->emit();
}

template<class Dispatcher, class Mutex>
void DispatcherMiddleman<Dispatcher, Mutex>::update_prof_apply_text(const std::string &text)
{
  CallData data(PROFILES_TEXT, text, false);
  queue.push(data);
  dispatch->emit();
}

// Receive method (called from main thread)
template<class Dispatcher, class Mutex>
void DispatcherMiddleman<Dispatcher, Mutex>::handle_signal()
{
  CallData data = queue.pop();

  switch (data.type) {
    case PROFILE:
      prof(data.string);
      break;

    case PROCESS:
      proc(data.string);
      break;

    case LOGS:
      logs(data.string);
      break;

    case PROFILES_TEXT:
      std::cerr << data.string << std::endl;
      break;

    case NONE:
      // Do nothing...
      break;
  }

  // If there was an error with pkexec, show the prompt asking to reauthenticate
  show_reauth(data.had_authentication_error);
}

// Used to avoid linker errors
// For more information, see: https://isocpp.org/wiki/faq/templates#class-templates
template class DispatcherMiddleman<Glib::Dispatcher, std::mutex>;
