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
void DispatcherMiddleman<Dispatcher, Mutex>::update_profiles(Json::Value &value)
{
  CallData data(PROFILE, value);
  queue.push(data);
  dispatch->emit();
}

template<class Dispatcher, class Mutex>
void DispatcherMiddleman<Dispatcher, Mutex>::update_processes(Json::Value &value)
{
  CallData data(PROCESS, value);
  queue.push(data);
  dispatch->emit();
}

template<class Dispatcher, class Mutex>
void DispatcherMiddleman<Dispatcher, Mutex>::update_logs(Json::Value &value)
{
  CallData data(LOGS, value);
  queue.push(data);
  dispatch->emit();
}

template<class Dispatcher, class Mutex>
void DispatcherMiddleman<Dispatcher, Mutex>::update_prof_apply_text(const std::string &text)
{
  CallData data(PROFILES_TEXT, text);
  queue.push(data);
  dispatch->emit();
}

template<class Dispatcher, class Mutex>
void DispatcherMiddleman<Dispatcher, Mutex>::update_reauth(const bool &should_reauth)
{
  CallData data(should_reauth);
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
      prof(data.data);
      break;

    case PROCESS:
      proc(data.data);
      break;

    case LOGS:
      logs(data.data);
      break;

    case PROFILES_TEXT:
      std::cerr << data.data << std::endl;
      break;

    case REAUTH:
      show_reauth(data.should_reauth);
      break;

    case NONE:
      // Do nothing...
      break;
  }
}

// Used to avoid linker errors
// For more information, see: https://isocpp.org/wiki/faq/templates#class-templates
template class DispatcherMiddleman<Glib::Dispatcher, std::mutex>;
