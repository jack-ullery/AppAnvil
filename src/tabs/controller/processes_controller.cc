#include "processes_controller.h"
#include "../model/status_column_record.h"

#include <string>

// clang-tidy throws [cert-err58-cpp], but it's not a problem in this case, so lets ignore it.
const std::regex unconfined_proc("^\\s*(\\S+)\\s+(\\S+)\\s+(\\S+)\\s+(unconfined|\\S+ \\(\\S+\\))\\s+(\\S+)"); // NOLINT(cert-err58-cpp)

template<class ColumnRecord> 
void ProcessesController<ColumnRecord>::add_row_from_line(const std::shared_ptr<ColumnRecord> &col_record, const std::string &line)
{
  //auto row = col_record->new_row();
  Gtk::TreeRow row;

  std::smatch match;
  std::regex_search(line, match, unconfined_proc);

  unsigned int pid   = stoul(match[1]); // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  unsigned int ppid  = stoul(match[2]); // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  std::string user   = match[3];        // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  std::string status = match[4];        // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  std::string comm   = match[5];        // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

  if(ppid > 0) {
    auto parent_row = col_record->get_parent_by_pid(ppid);
    row             = col_record->new_child_row(parent_row);
  } else {
    row = col_record->new_row();
  }

  row->set_value(0, comm);   // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  row->set_value(1, user);   // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  row->set_value(2, pid);    // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  row->set_value(3, status); // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
}

template<class ColumnRecord> 
void ProcessesController<ColumnRecord>::add_data_to_record(const std::string &unconfined)
{
  // Delete all the data from col_record
  col_record->clear();

  std::stringstream data;
  data << unconfined;

  std::string line;
  while(std::getline(data, line)) {
    add_row_from_line(col_record, line);
  }

  col_record->reselect_rows();
  refresh();
}

template<class ColumnRecord> 
ProcessesController<ColumnRecord>::ProcessesController() : col_record{ColumnRecord::create(Status::get_view(), Status::get_window(), col_names)}
{
  auto filter_fun = sigc::mem_fun(*this, &ProcessesController<ColumnRecord>::filter);
  col_record->set_visible_func(filter_fun);
}

// Used to avoid linker errors
// For more information, see: https://isocpp.org/wiki/faq/templates#class-templates
template class ProcessesController<StatusColumnRecord>;