#ifndef GTKMM_EXAMPLE_LOGS_H
#define GTKMM_EXAMPLE_LOGS_H

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
#include <regex>
#include <string>
#include <vector>

#define UNKNOWN_STATUS "unknown"

class Logs : public Status
{
  public:
    Logs();
    void refresh();
  
  protected:
    // Signal handlers    
    void on_search_changed();

    const std::vector<std::string> col_names{"Type", "Operation", "Name", "Pid", "Status"};
    std::shared_ptr<StatusColumnRecord> col_record;

  private:
    void add_row_from_line(const std::string& line);
    static std::string parse_line(const std::string& line, const std::regex& elem);
};

#endif // GTKMM_EXAMPLE_LOGS_H
