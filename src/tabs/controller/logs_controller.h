#ifndef TABS_CONTROLLER_LOGSCONTROLLER_H
#define TABS_CONTROLLER_LOGSCONTROLLER_H

#include "../model/status_column_record.h"
#include "jsoncpp/json/json.h"
#include "status_controller.h"

#include <iomanip>
#include <memory>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

template<class LogsTab, class ColumnRecord> class LogsController : public StatusController<LogsTab>
{
public:
  // For production
  explicit LogsController();
  // For unit testing
  explicit LogsController(std::shared_ptr<ColumnRecord> column_record, std::shared_ptr<LogsTab> logs);

  virtual void add_data_to_record(const std::string &data);
  static void add_row_from_json(const std::shared_ptr<ColumnRecord> &col_record, const Json::Value &entry);
  static std::string format_log_data(const std::string &data);
  static std::string format_timestamp(const time_t &timestamp);
  unsigned int num_visible_rows(); 
  void refresh();

protected:
  struct LogData {
    time_t timestamp;
    std::string type;
    std::string operation;
    std::string name;
    std::string pid;
    std::string status;

    LogData(time_t a, std::string b, std::string c, std::string d, std::string e, std::string f)
        : timestamp{a}, type{std::move(b)}, operation{std::move(c)}, name{std::move(d)}, pid{std::move(e)}, status{std::move(f)}
    {
    }
  };

private:
  std::shared_ptr<LogsTab> logs;

  const std::vector<ColumnHeader> col_names{ColumnHeader("Time"),
                                            ColumnHeader("Type"),
                                            ColumnHeader("Operation"),
                                            ColumnHeader("Name"),
                                            ColumnHeader("Pid", ColumnHeader::ColumnType::INT),
                                            ColumnHeader("Status")};
  std::shared_ptr<ColumnRecord> col_record;
};

#endif // TABS_CONTROLLER_LOGSCONTROLLER_H
