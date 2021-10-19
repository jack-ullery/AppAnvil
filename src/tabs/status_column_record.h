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
        
        /**
         * @brief Create a new StatusColumnRecord object.
         * 
         * @details
         * This method creates a new StatusColumnRecord and an associated TreeStore object.
         * It then sets the TreeStore object as the model for the `view` argument (if `view` already has an underlying model, this will replace that model).
         * Then columns are created for each element of `names` with titles from that std::vector.
         * 
         * @param view, The TreeView object to be backed by the new StatusColumnRecord object
         * @param names, The vector of names which will be the titles of columns in the StatusColumnRecord object.
         * 
         * @returns std::shared_ptr refrencing a new StatusColumnRecord object.
         */
        static std::shared_ptr<StatusColumnRecord> create(const std::shared_ptr<Gtk::TreeView>& view, std::vector<std::string> names);

        /**
         * @brief Creates and returns a new TreeRow in the table.
         * 
         * @details
         * Data of the row can be modified using the `set_row_data()` method
         * 
         * @returns A new TreeRow from the table.
         */
        Gtk::TreeRow new_row();

        /**
         * @brief Creates and returns a new TreeRow in the table which is the child of another TreeRow.
         * 
         * @details
         * Data of the row can be modified using the `set_row_data()` method
         * 
         * @param parent, The TreeRow to be the parent of the created TreeRow
         * 
         * @returns A new TreeRow from the table.
         */
        Gtk::TreeRow new_child_row(const Gtk::TreeRow& parent);

        /**
         * @brief Sets the string in the specified column of a TreeRow object.
         * 
         * @details
         * Sets the `index` column of `row` to be equal to `data`.
         * Throws an error if `index` is out of bounds (index < 0 or index > number of columns).
         * 
         * @param row, The row to modify
         * @param index, The column number to access
         * @param data, The string to put in the column
         * 
         * @throws std::out_of_range, if `index` does not point to a column in our column record
         * 
         * @returns A new TreeRow from the table.
         */
        void set_row_data(const Gtk::TreeRow& row, const uint& index, const std::string& data);

        /**
         * @brief Deletes all rows in the StatusColumnRecord.
         */
        void clear();

    private:
        explicit StatusColumnRecord(const std::vector<std::string>& names);
        std::vector<Gtk::TreeModelColumn<std::string>> column;
        Glib::RefPtr<Gtk::TreeStore> store;

};

#endif // TABS_STATUS_COLUMN_RECORD_H