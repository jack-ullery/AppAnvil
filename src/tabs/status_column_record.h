#ifndef TABS_STATUS_COLUMN_RECORD_H
#define TABS_STATUS_COLUMN_RECORD_H

#include <gtkmm/treemodel.h>
#include <gtkmm/treemodelcolumn.h>
#include <gtkmm/treestore.h>
#include <gtkmm/treeview.h>
#include <memory>

constexpr int MIN_COL_WIDTH = 20;

class StatusColumnRecord : public Gtk::TreeModel::ColumnRecord
{
    public:
        static std::shared_ptr<StatusColumnRecord> create(const std::shared_ptr<Gtk::TreeView>& view, std::vector<std::string> names);

        Gtk::TreeRow new_column();
        Gtk::TreeRow new_child_column(const Gtk::TreeRow& parent);
        void set_row_column_data(const Gtk::TreeRow& row, const uint& index, const std::string& data);
        void clear();

    private:
        explicit StatusColumnRecord(const std::vector<std::string>& names);
        std::vector<Gtk::TreeModelColumn<std::string>> column;
        Glib::RefPtr<Gtk::TreeStore> store;

};

#endif // TABS_STATUS_COLUMN_RECORD_H