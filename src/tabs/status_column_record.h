#ifndef GTKMM_EXAMPLE_STATUS_COLUMN_RECORD_H
#define GTKMM_EXAMPLE_STATUS_COLUMN_RECORD_H

#include <gtkmm/treemodel.h>
#include <gtkmm/treemodelcolumn.h>
#include <gtkmm/treestore.h>
#include <gtkmm/treeview.h>
#include <memory>

#define MIN_COL_WIDTH 20

class StatusColumnRecord : public Gtk::TreeModel::ColumnRecord
{
    public:
        static std::shared_ptr<StatusColumnRecord> create(const std::shared_ptr<Gtk::TreeView>& view, std::vector<std::string> names);
        Gtk::TreeRow new_column();
        Gtk::TreeRow new_child_column(const Gtk::TreeRow& parent);
        void clear();

        std::vector<Gtk::TreeModelColumn<std::string>> column;

    protected:
        StatusColumnRecord(const std::vector<std::string>& names);
        Glib::RefPtr<Gtk::TreeStore> store;
};

#endif // GTKMM_EXAMPLE_STATUS_COLUMN_RECORD_H