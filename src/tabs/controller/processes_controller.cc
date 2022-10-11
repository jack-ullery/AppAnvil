#include "processes_controller.h"
#include "../model/database.h"
#include "../model/process_adapter.h"
#include "../view/processes.h"
#include "status_controller.h"

#include <iostream>
#include <regex>
#include <string>

// clang-tidy throws [cert-err58-cpp], but it's not a problem in this case, so lets ignore it.
const std::regex unconfined_proc("^\\s*(\\S+)\\s+(\\S+)\\s+(\\S+)\\s+(unconfined|\\S+ \\(\\S+\\))\\s+(\\S+)"); // NOLINT(cert-err58-cpp)

// TODO(regex): This is pretty buggy currently, we should fix it
const std::regex confined_prof("^\\s*(.+)\\s+\\((enforce|complain)\\)"); // NOLINT(cert-err58-cpp)

template<class ProcessesTab, class Database, class Adapter>
void ProcessesController<ProcessesTab, Database, Adapter>::add_row_from_line(const std::string &line)
{
  std::smatch match;
  std::regex_search(line, match, unconfined_proc);

  unsigned int pid    = stoul(match[1]); // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  unsigned int ppid   = stoul(match[2]); // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  std::string user    = match[3];        // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  std::string status  = match[4];        // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  std::string process = match[5];        // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

  // Attempt to get the profile that confines this process (if availible)
  std::string profile;
  bool is_confined = std::regex_match(status, match, confined_prof);
  if (is_confined) {
    profile = match[1]; // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  }

  adapter.put_data(process, profile, pid, ppid, user, status);
}

template<class ProcessesTab, class Database, class Adapter>
void ProcessesController<ProcessesTab, Database, Adapter>::add_data_to_record(const std::string &unconfined)
{
  std::stringstream data;
  data << unconfined;

  std::string line;
  while (std::getline(data, line)) {
    add_row_from_line(line);
  }

  refresh();
}

template<class ProcessesTab, class Database, class Adapter>
void ProcessesController<ProcessesTab, Database, Adapter>::refresh()
{
  uint num_visible = adapter.get_col_record()->filter_rows();
  proc->set_status_label_text(" " + std::to_string(num_visible) + " matching processes");
}

template<class ProcessesTab, class Database, class Adapter>
ProcessesController<ProcessesTab, Database, Adapter>::ProcessesController(std::shared_ptr<Database> database)
  : proc{ StatusController<ProcessesTab>::get_tab() },
    adapter(database, proc->get_view(), proc->get_window())
{
  // Set the Processes<ColumnRecord>::refresh function to be called whenever
  // the searchbar and checkboxes are updated
  auto func = sigc::mem_fun(*this, &ProcessesController<ProcessesTab, Database, Adapter>::refresh);
  proc->set_refresh_signal_handler(func);

  auto filter_fun = sigc::mem_fun(*this, &ProcessesController<ProcessesTab, Database, Adapter>::filter);
  adapter.get_col_record()->set_visible_func(filter_fun);
}

// Used to avoid linker errors
// For more information, see: https://isocpp.org/wiki/faq/templates#class-templates
template class ProcessesController<Processes, Database, ProcessAdapter<Database, StatusColumnRecord>>;