#ifndef TABS_STATUS_COLUMN_RECORD_H
#define TABS_STATUS_COLUMN_RECORD_H

#include <cstddef>
#include <gtkmm/treemodel.h>
#include <gtkmm/treemodelcolumn.h>
#include <gtkmm/treemodelfilter.h>
#include <gtkmm/treestore.h>
#include <gtkmm/treeview.h>
#include <memory>

constexpr int MIN_COL_WIDTH = 20;

struct ColumnHeader {
  enum ColumnType { STRING, INT };

  ColumnType type;
  std::string name;

  ColumnHeader(std::string name, ColumnType type) : type{type}, name{std::move(name)} { }

  explicit ColumnHeader(std::string name) : type{STRING}, name{std::move(name)} { }
};

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
  static std::shared_ptr<StatusColumnRecord> create(const std::shared_ptr<Gtk::TreeView> &view, const std::vector<ColumnHeader> &names);

  /**
   * @brief Sets the callback function which specifies whether a row should be visible
   *
   * @details
   * Sets the callback function which specifies whether a row should be visible.
   * Function should return true if row should be visible, or false otherwise.
   * This is used do decide which rows should be visible.
   *
   * @param filter, The callback function to use
   */
  void set_visible_func(const Gtk::TreeModelFilter::SlotVisible &filter);

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
  Gtk::TreeRow new_child_row(const Gtk::TreeRow &parent);

  /**
   * @brief Deletes all rows in the StatusColumnRecord, while remembering which row was selected.
   */
  void clear();

  /**
   * @brief Set the visibility all the rows in the ColumnRecord.
   *
   * @details
   * Sets the visibility of all the rows in the column record based on the function parameter
   * of `set_visible_function`. If no valid function was specified through that method, before calling this
   * method, then this will have undefined behavior.
   *
   * @returns The number of visible rows.
   */
  uint filter_rows();

private:
  explicit StatusColumnRecord(const std::shared_ptr<Gtk::TreeView> &view, const std::vector<ColumnHeader> &names);

  /**
   * @brief Selects the previously selected row before `clear()` was called.
   *
   * @details
   * Selects the first valid row if it matches the row that was previously selected the last time `StatusColumnRecord::clear()` was called.
   * If either it didn't match any row or `clear()` was never called on this ColumnRecord, then no row is selected.
   */
  void reselect_row();

  std::vector<std::string> row_to_vector(const Gtk::TreeRow& row);

  std::vector<Gtk::TreeModelColumn<std::string>> column;
  Glib::RefPtr<Gtk::TreeStore> store;
  Glib::RefPtr<Gtk::TreeModelFilter> filter_model;
  Glib::RefPtr<Gtk::TreeModelSort> sort_model;
  Gtk::TreeModelFilter::SlotVisible filter_fun;

  std::shared_ptr<Gtk::TreeView> view;
  std::vector<std::string> last_selected_row;

  static bool default_filter(const Gtk::TreeModel::iterator& node);
};

#endif // TABS_STATUS_COLUMN_RECORD_H