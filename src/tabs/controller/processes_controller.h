#ifndef TABS_CONTROLLER_PROCESSES_CONTROLLER_H
#define TABS_CONTROLLER_PROCESSES_CONTROLLER_H

#include "status_controller.h"

#include <memory>
#include <regex>
#include <string>

template<class ProcessesTab, class Database, class Adapter>
class ProcessesController : public StatusController<ProcessesTab>
{
public:
  explicit ProcessesController(std::shared_ptr<Database> database);

  virtual void add_data_to_record(const std::string &unconfined);
  void refresh();

protected:
  void add_row_from_line(const std::string &line);

private:
  std::shared_ptr<ProcessesTab> proc;
  Adapter adapter;
};

#endif // TABS_CONTROLLER_PROCESSES_CONTROLLER_H
