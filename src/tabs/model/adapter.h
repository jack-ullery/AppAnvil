#ifndef TABS_MODEL_ADAPTER_H
#define TABS_MODEL_ADAPTER_H

#include "status_column_record.h"

class Adapter
{
  public:
    virtual StatusColumnRecord get_col_record();
};

#endif // TABS_MODEL_ADAPTER_H
