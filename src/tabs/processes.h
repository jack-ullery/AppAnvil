#ifndef GTKMM_EXAMPLE_PROCESSES_H
#define GTKMM_EXAMPLE_PROCESSES_H

#include "status.h"

#include <gtkmm/box.h>
#include <gtkmm/builder.h>
#include <gtkmm/treemodel.h>
#include <gtkmm/treemodelcolumn.h>
#include <gtkmm/treestore.h>
#include <gtkmm/treeview.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/searchentry.h>
#include <memory>
#include <string>

class Processes : public Status
{
  public:
    Processes();
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
          add(s_process);
          add(s_profile);
          add(s_status);
        }
        // Will need to change these columns
        Gtk::TreeModelColumn<std::string> s_process;
        Gtk::TreeModelColumn<std::string> s_profile;
        Gtk::TreeModelColumn<std::string> s_status;
        // Gtk::TreeModelColumn<Gtk::TreeModel::ColumnRecord> s_row;
    };

    StatusColumnRecord s_record;
    Glib::RefPtr<Gtk::TreeStore> tree_store;
};

#endif // GTKMM_EXAMPLE_PROCESSES_H
