#ifndef TABS_VIEW_PROCESSES_H
#define TABS_VIEW_PROCESSES_H

#include "../controller/processes_controller.h"
#include "status.h"

#include <memory>
#include <regex>
#include <string>

template<class ColumnRecord> class Processes : public Status
{
public:
  explicit Processes();

protected:
  static void add_row_from_line(const std::shared_ptr<ColumnRecord> &col_record, const std::string &line);
};

#endif // TABS_VIEW_PROCESSES_H
