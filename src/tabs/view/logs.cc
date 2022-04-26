#include "logs.h"

// For production
Logs::Logs(LogsController<Logs, StatusColumnRecord> control)
{
  auto func = sigc::mem_fun(control, &LogsController<Logs, StatusColumnRecord>::refresh);
  Status::set_refresh_signal_handler(func);

  Status::remove_status_selection();

  this->show_all();
}

// Used to avoid linker errors
// For more information, see: https://isocpp.org/wiki/faq/templates#class-templates
// template class Logs<StatusColumnRecord>;
