#include "logs.h"

// For production
Logs::Logs()
{
  Status::remove_status_selection();
  this->show_all();
}
