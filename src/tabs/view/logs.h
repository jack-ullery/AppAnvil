#ifndef TABS_VIEW_LOGS_H
#define TABS_VIEW_LOGS_H

#include "../controller/logs_controller.h"
#include "../model/status_column_record.h"
#include "jsoncpp/json/json.h"
#include "status.h"

#include <iomanip>
#include <memory>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

class Logs : public Status
{
public:
  // For production
  explicit Logs(LogsController<Logs, StatusColumnRecord> control);

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
};

#endif // TABS_VIEW_LOGS_H