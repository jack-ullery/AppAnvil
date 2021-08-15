#ifndef GTKMM_EXAMPLE_PROFILES_H
#define GTKMM_EXAMPLE_PROFILES_H

#include <gtkmm/liststore.h>
#include <gtkmm/treemodel.h>
#include <gtkmm/treemodelcolumn.h>
#include <gtkmm/treeview.h>
#include <gtkmm/scrolledwindow.h>
#include <memory>
#include <string>

#define UNKNOWN_STATUS "unknown"

class Profiles : public Gtk::ScrolledWindow
{
  public:
    Profiles();
    virtual ~Profiles();
    void refresh();
  
  protected:
    Gtk::TreeView s_view;

    class StatusColumnRecord : public Gtk::TreeModel::ColumnRecord
    {
      public:
        StatusColumnRecord()
        {
          add(s_profile);
          add(s_status);
        }
        Gtk::TreeModelColumn<std::string> s_profile;
        Gtk::TreeModelColumn<std::string> s_status;
    };

    StatusColumnRecord s_record;
    Glib::RefPtr<Gtk::ListStore> s_model;
};

#endif // GTKMM_EXAMPLE_PROFILES_H
