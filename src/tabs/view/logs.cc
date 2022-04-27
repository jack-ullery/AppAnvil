#include "logs.h"

// For production
Logs::Logs()
{
  Status::remove_status_selection();

  this->show_all();
}

// Used to avoid linker errors
// For more information, see: https://isocpp.org/wiki/faq/templates#class-templates
// template class Logs<StatusColumnRecord>;
