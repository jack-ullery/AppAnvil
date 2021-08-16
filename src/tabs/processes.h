#ifndef GTKMM_EXAMPLE_PROCESSES_H
#define GTKMM_EXAMPLE_PROCESSES_H

#include <gtkmm/liststore.h>
#include <gtkmm/treemodel.h>
#include <gtkmm/treemodelcolumn.h>
#include <gtkmm/treeview.h>
#include <gtkmm/scrolledwindow.h>
#include <memory>
#include <string>

class Processes : public Gtk::ScrolledWindow
{
  public:
    Processes();
    void refresh();  

  protected:
    Gtk::TreeView s_view;

    class StatusColumnRecord : public Gtk::TreeModel::ColumnRecord
    {
      public:
        StatusColumnRecord()
        {
          add(s_process);
          add(s_status);
        }
        // Will need to change these columns
        Gtk::TreeModelColumn<std::string> s_process;
        Gtk::TreeModelColumn<std::string> s_status;
    };

    StatusColumnRecord s_record;
    Glib::RefPtr<Gtk::ListStore> s_model;
};

#endif // GTKMM_EXAMPLE_PROCESSES_H
