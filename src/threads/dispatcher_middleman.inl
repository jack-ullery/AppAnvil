template<class Profiles, class Processes, class Logs, class Dispatcher, class Mutex>
DispatcherMiddleman<Profiles, Processes, Logs, Dispatcher, Mutex>::DispatcherMiddleman(std::shared_ptr<Profiles> prof_arg, std::shared_ptr<Processes> proc_arg, std::shared_ptr<Logs> logs_arg, std::shared_ptr<FileChooser> file_chooser_arg)
: dispatch{new Dispatcher()},
  state(NONE),
  data1(""),
  data2(""),
  prof{std::move(prof_arg)},
  proc{std::move(proc_arg)},
  logs{std::move(logs_arg)},
  file_chooser{std::move(file_chooser_arg)},
  mtx{new Mutex()}
{
  auto function = sigc::mem_fun(*this, &DispatcherMiddleman<Profiles, Processes, Logs, Dispatcher, Mutex>::handle_signal);
  dispatch->connect(function);
}

template<class Profiles, class Processes, class Logs, class Dispatcher, class Mutex>
DispatcherMiddleman<Profiles, Processes, Logs, Dispatcher, Mutex>::DispatcherMiddleman(std::shared_ptr<Dispatcher> disp, std::shared_ptr<Profiles> prof_arg, std::shared_ptr<Processes> proc_arg, std::shared_ptr<Logs> logs_arg,
                                          std::shared_ptr<Mutex> my_mtx)
: dispatch{std::move(disp)},
  state(NONE),
  data1(""),
  data2(""),
  prof{std::move(prof_arg)},
  proc{std::move(proc_arg)},
  logs{std::move(logs_arg)},
  mtx{std::move(my_mtx)}
{ }

// Send methods (called from second thread)
template<class Profiles, class Processes, class Logs, class Dispatcher, class Mutex>
void DispatcherMiddleman<Profiles, Processes, Logs, Dispatcher, Mutex>::update_profiles(const std::string& confined){
  std::lock_guard<Mutex> lock(*mtx);
  state = PROFILE;
  data1 = confined;
  data2 = "";
  dispatch->emit();
}

template<class Profiles, class Processes, class Logs, class Dispatcher, class Mutex>
void DispatcherMiddleman<Profiles, Processes, Logs, Dispatcher, Mutex>::update_processes(const std::string& confined, const std::string& unconfined){
  std::lock_guard<Mutex> lock(*mtx);
  state = PROCESS;
  data1 = confined;
  data2 = unconfined;
  dispatch->emit();
}

template<class Profiles, class Processes, class Logs, class Dispatcher, class Mutex>
void DispatcherMiddleman<Profiles, Processes, Logs, Dispatcher, Mutex>::update_logs(const std::string& logs){
  std::lock_guard<Mutex> lock(*mtx);
  state = LOGS;
  data1 = logs;
  data2 = "";
  dispatch->emit();
}

template<class Profiles, class Processes, class Logs, class Dispatcher, class Mutex>
void DispatcherMiddleman<Profiles, Processes, Logs, Dispatcher, Mutex>::update_prof_apply_text(const std::string& text){
  std::lock_guard<Mutex> lock(*mtx);
  state = PROFILES_TEXT;
  data1 = text;
  data2 = "";
  dispatch->emit();
}

// Receive method (called from main thread)
template<class Profiles, class Processes, class Logs, class Dispatcher, class Mutex>
void DispatcherMiddleman<Profiles, Processes, Logs, Dispatcher, Mutex>::handle_signal(){
  // Will need to use these later and we want them declared outside the following scope.
  CallState cached_state = NONE;
  std::string cached_data1;
  std::string cached_data2;
  // Only need the lock in this small scope to retrieve data.
  // Don't need to lock when calling the other methods in the main loop.
  {
    std::lock_guard<Mutex> lock(*mtx);
    cached_state = state;
    cached_data1 = data1;
    cached_data2 = data2;
  }

  switch (cached_state)
  {
    case PROFILE:
      prof->add_data_to_record(cached_data1);
      break;
    case PROCESS:
      proc->add_data_to_record(cached_data1, cached_data2);      
      break;
    case LOGS:
      logs->add_data_to_record(cached_data1);
      break;
    case FILECHOOSER:
      //file_chooser->add_data_to_record(cached_data1);
      break;
    case PROFILES_TEXT:
      prof->set_apply_label_text(cached_data1);
      break;
    case NONE:
      // Do nothing...
      break;
  }
}
