#include "processes.h"

#include <string>

Processes::Processes()
  : Status("/status.glade", "running processes")
{
  this->show_all();
}