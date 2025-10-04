#ifndef TABS_CONTROLLER_LOGS_CONTROLLER_H
#define TABS_CONTROLLER_LOGS_CONTROLLER_H

#include "status_controller.h"

#include <memory>
#include <string>

#ifdef TESTS_ENABLED
#include <gtest/gtest.h>
#endif

template<class LogsTab, class Database, class Adapter, class LogRecord>
class LogsController : public StatusController<LogsTab>
{
public:
  // For production
  explicit LogsController(std::shared_ptr<Database> database);
  // For unit testing
  explicit LogsController(std::shared_ptr<Adapter> adapter, std::shared_ptr<LogsTab> logs);

  void handle_log_selected();
  bool on_key_event(GdkEventKey *event);
  bool on_button_event(GdkEventButton *event);

  virtual void add_data_to_record(const std::list<std::shared_ptr<LogRecord>> &data);
  virtual void add_data_to_record_2(const std::string &json_data);

  unsigned int num_visible_rows();
  void refresh();

protected:
  // TODO(maybe remove): This removes quotes from data (which we may not need to do anymore)
  static std::string format_log_data(const std::string &data);

  bool add_data_to_record_helper(std::shared_ptr<std::list<std::shared_ptr<LogRecord>>> data);
  void add_row(const std::shared_ptr<LogRecord> &record);

private:
  std::shared_ptr<LogsTab> logs;
  std::shared_ptr<Adapter> adapter;

#ifdef TESTS_ENABLED
  FRIEND_TEST(LogAdapterTest, TEST_FORMAT_LOG_DATA);
#endif
};

#endif // TABS_CONTROLLER_LOGS_CONTROLLER_H
