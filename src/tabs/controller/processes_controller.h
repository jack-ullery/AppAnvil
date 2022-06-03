#ifndef TABS_CONTROLLER_PROCESSESCONTROLLER_H
#define TABS_CONTROLLER_PROCESSESCONTROLLER_H

#include "status_controller.h"

#include <memory>
#include <regex>
#include <string>

template<class ProcessesTab, class ColumnRecord> class ProcessesController : public StatusController<ProcessesTab>
{
public:
  ProcessesController();

  virtual void add_data_to_record(const std::string &unconfined);
  void refresh();

protected:
  static void add_row_from_line(const std::shared_ptr<ColumnRecord> &col_record, const std::string &line);

private:
  std::shared_ptr<ProcessesTab> proc;

  const std::vector<ColumnHeader> col_names
  {
    ColumnHeader("Process Name"), 
    ColumnHeader("User"),
    ColumnHeader("Pid", ColumnHeader::ColumnType::INT), 
    ColumnHeader("Status")
  };

  std::shared_ptr<ColumnRecord> col_record;
};

#endif // TABS_CONTROLLER_PROCESSESCONTROLLER_H
