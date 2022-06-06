#include "logs_controller.h"
#include "../model/database.h"
#include "../model/status_column_record.h"
#include "../view/logs.h"
#include <memory>

template<class LogsTab, class Database, class Adapter>
void LogsController<LogsTab, Database, Adapter>::add_row_from_json(const Json::Value &entry)
{
  // getting timestamp from json argument, retrieving important fields from json
  const time_t timestamp      = std::stol(entry["_SOURCE_REALTIME_TIMESTAMP"].asString()) / 1000000;
  const std::string type      = entry["_AUDIT_FIELD_APPARMOR"].asString();
  const std::string operation = entry["_AUDIT_FIELD_OPERATION"].asString();
  const std::string name      = entry["_AUDIT_FIELD_NAME"].asString();
  const std::string pid       = entry["_PID"].asString();
  const std::string status    = entry["_AUDIT_FIELD_PROFILE"].asString();

  adapter->put_data(timestamp,
                    type,
                    operation,
                    name,
                    stoul(pid),
                    status);
}

template<class LogsTab, class Database, class Adapter> void LogsController<LogsTab, Database, Adapter>::add_data_to_record(const std::string &data)
{

  // declare variables
  std::stringstream logs;
  std::istringstream json_data(data);
  std::string line;
  Json::Value root;
  Json::CharReaderBuilder builder;
  JSONCPP_STRING errs;

  // gets each log entry (in json format, separated by \n), parses the json, and calls add_row_from_json to add each individual entry
  while(std::getline(json_data, line)) {
    logs << line;
    if(!parseFromStream(builder, logs, &root, &errs)) {
      throw std::invalid_argument(errs + "\nArgument of add_data_to_record contains line with invalid JSON format.");
    }
    add_row_from_json(root);
    logs.clear();
    root.clear();
  }

  // refresh the display after all logs have been added
  refresh();
}

template<class LogsTab, class Database, class Adapter> void LogsController<LogsTab, Database, Adapter>::refresh()
{
  uint num_visible = adapter->get_col_record()->filter_rows();
  logs->set_status_label_text(" " + std::to_string(num_visible) + " logs");
}

// For unit testing
template<class LogsTab, class Database, class Adapter>
LogsController<LogsTab, Database, Adapter>::LogsController(std::shared_ptr<Adapter> adapter, std::shared_ptr<LogsTab> logs) 
  : logs{std::move(logs)},
    adapter{std::move(adapter)}  
  { }

// For production
template<class LogsTab, class Database, class Adapter>
LogsController<LogsTab, Database, Adapter>::LogsController(std::shared_ptr<Database> database)
  : logs{StatusController<LogsTab>::get_tab()},
    adapter{new Adapter(database, logs->get_view(), logs->get_window())}
{
  auto func = sigc::mem_fun(*this, &LogsController<LogsTab, Database, Adapter>::refresh);
  logs->set_refresh_signal_handler(func);

  auto filter_fun = sigc::mem_fun(*this, &LogsController::filter);
  adapter->get_col_record()->set_visible_func(filter_fun);
}

// Used to avoid linker errors
// For more information, see: https://isocpp.org/wiki/faq/templates#class-templates
template class LogsController<Logs, Database, LogAdapter<Database>>;
