#include "processes.h"

#include <string>

Processes::Processes()
  : Status("/resources/status.glade", "running processes")
{
  this->show_all();
}