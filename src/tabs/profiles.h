#ifndef GTKMM_EXAMPLE_PROFILES_H
#define GTKMM_EXAMPLE_PROFILES_H

#include "status.h"

#include <gtkmm/box.h>
#include <gtkmm/builder.h>
#include <gtkmm/enums.h>
#include <gtkmm/liststore.h>
#include <gtkmm/treemodelcolumn.h>
#include <gtkmm/treeview.h>
#include <gtkmm/treeviewcolumn.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/searchentry.h>
#include <memory>
#include <string>
#include <vector>

#define UNKNOWN_STATUS "unknown"

class Profiles : public Status
{
  public:
    Profiles();
    void refresh();
  
  protected:
    // Signal handlers    
    void on_search_changed();

    const std::vector<std::string> col_names{"Profile", "Status"};
    std::shared_ptr<StatusColumnRecord> col_record;
};

#endif // GTKMM_EXAMPLE_PROFILES_H
