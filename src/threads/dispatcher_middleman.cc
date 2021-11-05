#include "dispatcher_middleman.h"

DispatcherMiddleman::DispatcherMiddleman(std::shared_ptr<Profiles> prof_arg, std::shared_ptr<Processes> proc_arg, std::shared_ptr<Logs> logs_arg, std::shared_ptr<FileChooser> file_chooser_arg)
: dispatch(),
  state(NONE),
  data1(""),
  data2(""),
  prof(prof_arg),
  proc(proc_arg),
  logs(logs_arg),
  file_chooser(file_chooser_arg)
{
  auto function = sigc::mem_fun(*this, &DispatcherMiddleman::handle_signal);
  dispatch.connect(function);
}

// Send methods (called from second thread)
void DispatcherMiddleman::update_profiles(std::string confined){
  std::lock_guard<std::mutex> lock(mtx);
  state = PROFILE;
  data1 = confined;
  data2 = "";
  dispatch.emit();
}

void DispatcherMiddleman::update_processes(std::string confined, std::string unconfined){
  std::lock_guard<std::mutex> lock(mtx);
  state = PROCESS;
  data1 = confined;
  data2 = unconfined;
  dispatch.emit();
}

void DispatcherMiddleman::update_logs(std::string logs){
  std::lock_guard<std::mutex> lock(mtx);
  state = LOGS;
  data1 = logs;
  data2 = "";
  dispatch.emit();
}

// Receive method (called from main thread)
void DispatcherMiddleman::handle_signal(){
  // Will need to use these later and we want them declared outside the following scope.
  CallState cached_state;
  std::string cached_data1;
  std::string cached_data2;
  // Only need the lock in this small scope to retrieve data.
  // Don't need to lock when calling the other methods in the main loop.
  {
    std::lock_guard<std::mutex> lock(mtx);
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
      file_chooser->add_data_to_record(cached_data1);
      break;
    case NONE:
      // Do nothing...
      break;
  }
}
