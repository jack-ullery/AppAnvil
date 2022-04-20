#ifndef TABS_LOGS_H
#define TABS_LOGS_H

#include "../model/status_column_record.h"
#include "jsoncpp/json/json.h"
#include "status.h"

#include <iomanip>
#include <memory>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

template<class ColumnRecord> class Logs : public Status
{
public:
  // For production
  Logs();
  // For unit testing
  explicit Logs(std::shared_ptr<ColumnRecord> column_record);

  virtual void add_data_to_record(const std::string &data);
  static void add_row_from_json(const std::shared_ptr<ColumnRecord> &col_record, const Json::Value &entry);
  static std::string format_log_data(const std::string &data);
  static std::string format_timestamp(const time_t &timestamp);
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
  const std::vector<ColumnHeader> col_names{ColumnHeader("Time"),
                                            ColumnHeader("Type"),
                                            ColumnHeader("Operation"),
                                            ColumnHeader("Name"),
                                            ColumnHeader("Pid", ColumnHeader::ColumnType::INT),
                                            ColumnHeader("Status")};
  std::shared_ptr<ColumnRecord> col_record;
};

#include "logs.inl"

#endif // TABS_LOGS_H