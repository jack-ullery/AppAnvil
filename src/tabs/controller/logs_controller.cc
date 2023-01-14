#include "logs_controller.h"
#include "../../threads/log_record.h"
#include "../model/database.h"
#include "../model/status_column_record.h"
#include "../view/logs.h"

#include <glibmm/main.h>
#include <glibmm/priorities.h>
#include <list>
#include <memory>
#include <sigc++/functors/mem_fun.h>
#include <sstream>

template<class LogsTab, class Database, class Adapter, class LogRecord>
bool LogsController<LogsTab, Database, Adapter, LogRecord>::on_button_event(GdkEventButton *event)
{
  std::ignore = event;

  handle_log_selected();
  return false;
}

template<class LogsTab, class Database, class Adapter, class LogRecord>
bool LogsController<LogsTab, Database, Adapter, LogRecord>::on_key_event(GdkEventKey *event)
{
  std::ignore = event;

  handle_log_selected();
  return false;
}

template<class LogsTab, class Database, class Adapter, class LogRecord>
void LogsController<LogsTab, Database, Adapter, LogRecord>::handle_log_selected()
{
  // Check if there is any row selected
  auto selection    = logs->get_view()->get_selection();
  auto row_selected = selection->count_selected_rows() == 1;

  if (row_selected) {
    Gtk::TreePath path = selection->get_selected_rows()[0];
    Gtk::TreeRow row   = adapter->get_col_record()->get_row(path);

    LogTableEntry entry;
    row.get_value(0, entry);

    logs->set_information(entry.metadata);
  } else {
    std::list<std::pair<std::string, std::string>> empty_list;
    logs->set_information(empty_list);
  }
}

template<class LogsTab, class Database, class Adapter, class LogRecord>
std::string LogsController<LogsTab, Database, Adapter, LogRecord>::format_log_data(const std::string &data)
{
  const std::regex remove_quotes = std::regex("\\\"(\\S*)\\\"");
  std::smatch m;
  std::regex_search(data, m, remove_quotes);
  return m[1];
}

template<class LogsTab, class Database, class Adapter, class LogRecord>
void LogsController<LogsTab, Database, Adapter, LogRecord>::add_row(const std::shared_ptr<LogRecord> &record)
{
  // getting timestamp from json argument, retrieving important fields from json
  const time_t timestamp        = record->timestamp();
  const auto type               = record->event_type();
  const std::string type_string = record->event_type_string();
  const ulong pid               = record->pid();
  std::string operation         = record->operation();

  std::string profile_name;
  std::list<std::pair<std::string, std::string>> metadata = record->get_metadata();

  // Adapted from: https://gitlab.com/apparmor/apparmor/-/blob/master/libraries/libapparmor/include/aalogparse.h
  if (type == AA_RECORD_STATUS) {
    profile_name = record->name();

    std::string status = record->profile();
    metadata.emplace_front("Status", status);
  }
  else {
    profile_name = record->profile();

    std::string name = record->name();
    metadata.emplace_front("Name", name);
  }

  adapter->put_data(timestamp, type_string, operation, profile_name, pid, metadata);
}

template<class LogsTab, class Database, class Adapter, class LogRecord>
void LogsController<LogsTab, Database, Adapter, LogRecord>::add_data_to_record(const std::list<std::shared_ptr<LogRecord>> &data)
{
  auto lambda = [&, data]() -> bool { return add_data_to_record_helper(data.begin(), data.end()); };
  Glib::signal_idle().connect(lambda, Glib::PRIORITY_LOW);
}

template<class LogsTab, class Database, class Adapter, class LogRecord>
bool LogsController<LogsTab, Database, Adapter, LogRecord>::add_data_to_record_helper(const record_iter &begin, const record_iter &end)
{
  typename std::list<std::shared_ptr<LogRecord>>::const_iterator iter = begin;

  // gets each log entry (in json format, separated by \n), parses the json, and calls add_row_from_json to add each individual entry
  constexpr uint num_logs_batch = 127;
  for (uint i = 0; i < num_logs_batch; i++) {
    // Check if we exhausted the list of logs
    if (iter == end) {
      // Return false to disconnect the signal handler
      refresh();
      return false;
    }

    // Retrieve and pop the next log
    std::shared_ptr<LogRecord> log = *iter;
    iter++;

    add_row(log);
  }

  // Refresh the display to show an accurate count of the number of logs
  refresh();

  // Then continue signal processing
  auto lambda = [&, iter, end]() -> bool { return add_data_to_record_helper(iter, end); };
  Glib::signal_idle().connect(lambda, Glib::PRIORITY_LOW);

  // Disconnect the current signal handler
  return false;
}

template<class LogsTab, class Database, class Adapter, class LogRecord>
void LogsController<LogsTab, Database, Adapter, LogRecord>::refresh()
{
  uint num_visible = adapter->get_col_record()->filter_rows();
  logs->set_status_label_text(" " + std::to_string(num_visible) + " logs");
}

// For unit testing
template<class LogsTab, class Database, class Adapter, class LogRecord>
LogsController<LogsTab, Database, Adapter, LogRecord>::LogsController(std::shared_ptr<Adapter> adapter, std::shared_ptr<LogsTab> logs)
  : logs{ std::move(logs) },
    adapter{ std::move(adapter) }
{
}

// For production
template<class LogsTab, class Database, class Adapter, class LogRecord>
LogsController<LogsTab, Database, Adapter, LogRecord>::LogsController(std::shared_ptr<Database> database)
  : logs{ StatusController<LogsTab>::get_tab() },
    adapter{ new Adapter(database, logs->get_view(), logs->get_window()) }
{
  auto func = sigc::mem_fun(*this, &LogsController<LogsTab, Database, Adapter, LogRecord>::refresh);
  logs->set_refresh_signal_handler(func);

  auto filter_fun = sigc::mem_fun(*this, &LogsController::filter);
  adapter->get_col_record()->set_visible_func(filter_fun);

  // When a key/button is pressed or released, check if the selection has changed
  auto button_event_fun = sigc::mem_fun(*this, &LogsController::on_button_event);
  logs->get_view()->signal_button_release_event().connect(button_event_fun, true);

  auto key_event_fun = sigc::mem_fun(*this, &LogsController::on_key_event);
  logs->get_view()->signal_key_release_event().connect(key_event_fun, true);

  logs->get_view()->set_activate_on_single_click(true);
}

// Used to avoid linker errors
// For more information, see: https://isocpp.org/wiki/faq/templates#class-templates
template class LogsController<Logs, Database, LogAdapter<Database, StatusColumnRecord>, LogRecord>;
