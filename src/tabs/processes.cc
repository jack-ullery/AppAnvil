#include "processes.h"

#include <string>

// clang-tidy throws [cert-err58-cpp], but it's not a problem in this case, so lets ignore it.
const std::regex unconfined_proc("^\\s*(\\S+)\\s+(\\S+)\\s+(\\S+)\\s+(unconfined|\\S+ \\(\\S+\\))\\s+(\\S+)"); // NOLINT(cert-err58-cpp)

void Processes::add_row_from_line(const std::shared_ptr<StatusColumnRecord> &col_record, const std::string &line)
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

void Processes::add_data_to_record(const std::string &unconfined)
{
  // Delete all the data from col_record
  col_record->clear();

  std::stringstream data;
  data << unconfined;

  std::string line;
  while(std::getline(data, line)) {
    add_row_from_line(col_record, line);
  }

  refresh();
}

void Processes::refresh()
{
  uint num_visible = col_record->filter_rows();
  Status::set_status_label_text(" " + std::to_string(num_visible) + " matching processes");
}

Processes::Processes() : col_record{StatusColumnRecord::create(Status::get_view(), col_names)}
{
  // Set the Processes::refresh function to be called whenever
  // the searchbar and checkboxes are updated
  auto func = sigc::mem_fun(*this, &Processes::refresh);
  Status::set_refresh_signal_handler(func);

  auto filter_fun = sigc::mem_fun(*this, &Processes::filter);
  col_record->set_visible_func(filter_fun);

  Status::remove_status_selection();

  this->show_all();
}
