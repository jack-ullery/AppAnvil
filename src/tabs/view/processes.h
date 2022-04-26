#ifndef TABS_VIEW_PROCESSES_H
#define TABS_VIEW_PROCESSES_H

#include "status.h"

#include <memory>
#include <regex>
#include <string>

template<class ColumnRecord> class Processes : public Status
{
public:
  Processes();
  virtual void add_data_to_record(const std::string &unconfined);
  void refresh();

protected:
  // Signal handlers
  void on_search_changed();

  static void add_row_from_line(const std::shared_ptr<ColumnRecord> &col_record, const std::string &line);
};

#endif // TABS_VIEW_PROCESSES_H
