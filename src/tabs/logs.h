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
    void order_columns();
  
  protected:
    // Signal handlers    
    void on_search_changed();

    class StatusColumnRecord : public Gtk::TreeModel::ColumnRecord
    {
      public:
        StatusColumnRecord()
        {
          for(uint i = 0; i < column.size(); i++){
            column[i] = Gtk::TreeModelColumn<std::string>();
            add(column[i]);
          }
        }

      std::vector<Gtk::TreeModelColumn<std::string>> column = std::vector<Gtk::TreeModelColumn<std::string>>(5);
    };

    StatusColumnRecord col_record;
    Glib::RefPtr<Gtk::ListStore> list_store;

  private:
    void add_row_from_line(std::string line);
    static std::string parse_line(std::string line, std::regex elem);
};

#endif // GTKMM_EXAMPLE_LOGS_H
