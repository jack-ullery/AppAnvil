#ifndef TABS_LOGS_H
#define TABS_LOGS_H

#include "status.h"

#include <gtkmm/box.h>
#include <gtkmm/builder.h>
#include <gtkmm/enums.h>
#include <gtkmm/liststore.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/searchentry.h>
#include <gtkmm/treemodelcolumn.h>
#include <gtkmm/treeview.h>
#include <gtkmm/treeviewcolumn.h>
#include <memory>
#include <regex>
#include <string>
#include <vector>

class Logs : public Status
{
  public:
    Logs();
    void add_data_to_record(std::string data);
    void refresh();
    
  private:
    const std::vector<std::string> col_names{"Type", "Operation", "Name", "Pid", "Status"};
    std::shared_ptr<StatusColumnRecord> col_record;

    static void add_row_from_line(std::shared_ptr<StatusColumnRecord> col_record, const std::string& line);
    static std::string parse_line(const std::string& line, const std::regex& elem);
};

#endif // TABS_LOGS_H
