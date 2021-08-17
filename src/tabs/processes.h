#ifndef GTKMM_EXAMPLE_PROCESSES_H
#define GTKMM_EXAMPLE_PROCESSES_H

#include <gtkmm/box.h>
#include <gtkmm/builder.h>
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
    void order_columns();

  protected:
    // GUI Builder to parse UI from xml file
    Glib::RefPtr<Gtk::Builder> builder;

    // Member Widgets
    std::unique_ptr<Gtk::TreeView> s_view;
    std::unique_ptr<Gtk::ScrolledWindow> s_win;
    std::unique_ptr<Gtk::Box> s_box;

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

  private:
    template <typename T_Widget>
    static std::unique_ptr<T_Widget> get_widget(const Glib::ustring name, const Glib::RefPtr<Gtk::Builder> &builder);
};

#endif // GTKMM_EXAMPLE_PROCESSES_H
