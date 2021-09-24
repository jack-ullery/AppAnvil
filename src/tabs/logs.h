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
          add(log_col);
        }

      Gtk::TreeModelColumn<std::string> log_col;
    };

    StatusColumnRecord col_record;
    Glib::RefPtr<Gtk::ListStore> list_store;
};

#endif // GTKMM_EXAMPLE_LOGS_H
