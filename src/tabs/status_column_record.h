#ifndef TABS_STATUS_COLUMN_RECORD_H
#define TABS_STATUS_COLUMN_RECORD_H

#include <gtkmm/treemodel.h>
#include <gtkmm/treemodelcolumn.h>
#include <gtkmm/treemodelfilter.h>
#include <gtkmm/treestore.h>
#include <gtkmm/treeview.h>
#include <map>
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
   * @brief Deletes all rows in the StatusColumnRecord.
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

  void reselect_rows();

private:
  struct RowData {
    const bool isSelected;
    const bool isExpanded;

    RowData(const bool &isSelectedArg, const bool &isExpandedArg)
    : isSelected{isSelectedArg},
      isExpanded{isExpandedArg}
      { }
  };

  explicit StatusColumnRecord(const std::shared_ptr<Gtk::TreeView> &view, const std::vector<ColumnHeader> &names);
  Glib::RefPtr<Gtk::TreeStore> store;
  std::shared_ptr<Gtk::TreeView> view;
  Glib::RefPtr<Gtk::TreeModelFilter> filter_model;
  Glib::RefPtr<Gtk::TreeModelSort> sort_model;
  Gtk::TreeModelFilter::SlotVisible filter_fun;
  std::map<Gtk::TreePath, RowData> significant_rows;

  static bool default_filter(const Gtk::TreeModel::iterator &node);
};

#endif // TABS_STATUS_COLUMN_RECORD_H