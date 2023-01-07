#ifndef TABS_CONTROLLER_LOGSCONTROLLER_H
#define TABS_CONTROLLER_LOGSCONTROLLER_H

#include "../model/status_column_record.h"
#include "jsoncpp/json/json.h"
#include "status_controller.h"

#include <memory>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#ifdef TESTS_ENABLED
#include <gtest/gtest.h>
#endif

template<class LogsTab, class Database, class Adapter>
class LogsController : public StatusController<LogsTab>
{
public:
  // For production
  explicit LogsController(std::shared_ptr<Database> database);
  // For unit testing
  explicit LogsController(std::shared_ptr<Adapter> adapter, std::shared_ptr<LogsTab> logs);

  void handle_log_selected();
  bool on_key_event(GdkEvent *event);
  bool on_button_event(GdkEvent *event);

  virtual void add_data_to_record(const std::string &data);

  unsigned int num_visible_rows();
  void refresh();

protected:
  static std::string format_log_data(const std::string &data);

  bool add_data_to_record_helper(const std::shared_ptr<std::istringstream> &json_data);
  void add_row_from_json(const Json::Value &entry);

private:
  std::shared_ptr<LogsTab> logs;
  std::shared_ptr<Adapter> adapter;

#ifdef TESTS_ENABLED
  FRIEND_TEST(LogAdapterTest, TEST_FORMAT_LOG_DATA);
#endif
};

#endif // TABS_CONTROLLER_LOGSCONTROLLER_H
