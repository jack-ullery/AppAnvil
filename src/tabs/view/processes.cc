#include "processes.h"

#include <string>

Processes::Processes() 
{
  Status::remove_status_selection();
  this->show_all();
}