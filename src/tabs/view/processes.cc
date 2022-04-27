#include "processes.h"
#include "../model/status_column_record.h"

#include <string>

template<class ColumnRecord> 
Processes<ColumnRecord>::Processes() 
{
  Status::remove_status_selection();

  this->show_all();
}

// Used to avoid linker errors
// For more information, see: https://isocpp.org/wiki/faq/templates#class-templates
template class Processes<StatusColumnRecord>;