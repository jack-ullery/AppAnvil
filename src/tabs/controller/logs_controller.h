#ifndef TABS_CONTROLLER_LOGSCONTROLLER_H
#define TABS_CONTROLLER_LOGSCONTROLLER_H

#include "../model/status_column_record.h"
#include "jsoncpp/json/json.h"
#include "status_controller.h"

#include <memory>
#include <sstream>
#include <string>
#include <vector>

template<class LogsTab, class Database, class Adapter> class LogsController : public StatusController<LogsTab>
{
public:
  // For production
  explicit LogsController(std::shared_ptr<Database> database);
  // For unit testing
  explicit LogsController(std::shared_ptr<Adapter> column_record, std::shared_ptr<LogsTab> logs);

  virtual void add_data_to_record(const std::string &data);
  unsigned int num_visible_rows(); 
  void refresh();

protected:
  void add_row_from_json(const Json::Value &entry);

private:
  std::shared_ptr<LogsTab> logs;
  std::shared_ptr<Adapter> adapter;
};

#endif // TABS_CONTROLLER_LOGSCONTROLLER_H
