#ifndef GTKMM_EXAMPLE_PROCESSES_H
#define GTKMM_EXAMPLE_PROCESSES_H

#include <gtkmm/box.h>
#include <gtkmm/builder.h>
#include <gtkmm/treestore.h>
#include <gtkmm/treemodel.h>
#include <gtkmm/treemodelcolumn.h>
#include <gtkmm/treeview.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/searchentry.h>
#include <memory>
#include <string>

class Processes : public Gtk::ScrolledWindow
{
  public:
    Processes();
    void refresh(const std::string& rule);
    void order_columns();

  protected:
    // Signal handlers    
    void on_search_changed();

    // GUI Builder to parse UI from xml file
    Glib::RefPtr<Gtk::Builder> builder;

    // Member Widgets
    std::unique_ptr<Gtk::TreeView> s_view;
    std::unique_ptr<Gtk::ScrolledWindow> s_win;
    std::unique_ptr<Gtk::Box> s_box;
    std::unique_ptr<Gtk::SearchEntry> s_search;

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
    };

    StatusColumnRecord s_record;
    Glib::RefPtr<Gtk::TreeStore> tree_store;

  private:
    template <typename T_Widget>
    static std::unique_ptr<T_Widget> get_widget(const Glib::ustring name, const Glib::RefPtr<Gtk::Builder>& builder);    
    static bool filter(const std::string& str, const std::string& rule);

    std::unique_ptr<Gtk::TreeModel::ColumnRecord> make_column_record();
};

#endif // GTKMM_EXAMPLE_PROCESSES_H
