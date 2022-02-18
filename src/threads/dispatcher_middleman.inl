template<class Profiles, class Processes, class Logs, class Dispatcher, class Mutex>
DispatcherMiddleman<Profiles, Processes, Logs, Dispatcher, Mutex>::DispatcherMiddleman(std::shared_ptr<Profiles> prof_arg,
                                                                                       std::shared_ptr<Processes> proc_arg,
                                                                                       std::shared_ptr<Logs> logs_arg)
    : dispatch{new Dispatcher()}, prof{std::move(prof_arg)}, proc{std::move(proc_arg)}, logs{std::move(logs_arg)}
{
  auto function = sigc::mem_fun(*this, &DispatcherMiddleman<Profiles, Processes, Logs, Dispatcher, Mutex>::handle_signal);
  dispatch->connect(function);
}

template<class Profiles, class Processes, class Logs, class Dispatcher, class Mutex>
DispatcherMiddleman<Profiles, Processes, Logs, Dispatcher, Mutex>::DispatcherMiddleman(std::shared_ptr<Dispatcher> disp,
                                                                                       std::shared_ptr<Profiles> prof_arg,
                                                                                       std::shared_ptr<Processes> proc_arg,
                                                                                       std::shared_ptr<Logs> logs_arg,
                                                                                       std::shared_ptr<Mutex> my_mtx)
    : queue(std::deque<CallData>(), my_mtx), dispatch{std::move(disp)}, prof{std::move(prof_arg)}, proc{std::move(proc_arg)},
      logs{std::move(logs_arg)}
{
}

// Send methods (called from second thread)
template<class Profiles, class Processes, class Logs, class Dispatcher, class Mutex>
void DispatcherMiddleman<Profiles, Processes, Logs, Dispatcher, Mutex>::update_profiles(const std::string &confined)
{
  CallData data(PROFILE, confined);
  queue.push(data);
  dispatch->emit();
}

template<class Profiles, class Processes, class Logs, class Dispatcher, class Mutex>
void DispatcherMiddleman<Profiles, Processes, Logs, Dispatcher, Mutex>::update_processes(const std::string &confined,
                                                                                         const std::string &unconfined)
{
  CallData data(PROCESS, confined, unconfined);
  queue.push(data);
  dispatch->emit();
}

template<class Profiles, class Processes, class Logs, class Dispatcher, class Mutex>
void DispatcherMiddleman<Profiles, Processes, Logs, Dispatcher, Mutex>::update_logs(const std::string &logs)
{
  CallData data(LOGS, logs);
  queue.push(data);
  dispatch->emit();
}

template<class Profiles, class Processes, class Logs, class Dispatcher, class Mutex>
void DispatcherMiddleman<Profiles, Processes, Logs, Dispatcher, Mutex>::update_prof_apply_text(const std::string &text)
{
  CallData data(PROFILES_TEXT, text);
  queue.push(data);
  dispatch->emit();
}

// Receive method (called from main thread)
template<class Profiles, class Processes, class Logs, class Dispatcher, class Mutex>
void DispatcherMiddleman<Profiles, Processes, Logs, Dispatcher, Mutex>::handle_signal()
{

  CallData data = queue.pop();

  switch(data.type) {
  case PROFILE:
    prof->add_data_to_record(data.arg_1);
    break;

  case PROCESS:
    proc->add_data_to_record(data.arg_1, data.arg_2);
    break;

  case LOGS:
    logs->add_data_to_record(data.arg_1);
    break;

  case PROFILES_TEXT:
    prof->set_apply_label_text(data.arg_1);
    break;

  case NONE:
    // Do nothing...
    break;
  }
}
