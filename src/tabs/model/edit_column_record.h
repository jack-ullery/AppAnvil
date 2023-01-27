#ifndef TABS_EDIT_COLUMN_RECORD_H
#define TABS_EDIT_COLUMN_RECORD_H

#include "../column_header.h"
#include "../view/status.h"

#include <gtkmm/button.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/treeiter.h>
#include <gtkmm/treemodel.h>
#include <gtkmm/treemodelcolumn.h>
#include <gtkmm/treemodelfilter.h>
#include <gtkmm/treestore.h>
#include <gtkmm/treeview.h>
#include <initializer_list>
#include <map>
#include <memory>

class EditColumnRecord : public Gtk::TreeModel::ColumnRecord
{
public:
  constexpr static int MIN_COL_WIDTH = 20;

  static std::shared_ptr<EditColumnRecord> create(const std::shared_ptr<Gtk::TreeView> &view,
                                                  const std::vector<std::string> &names);

  /**
   * @brief Creates and returns a new row in the table.
   *
   * @details
   * Data of the row can be modified using the `set_row_data()` method
   *
   * @returns A new TreeRow from the table.
   */
  Gtk::TreeRow new_row();

  /**
   * @brief Gets the TreeRow in the table which is the child of another TreeRow.
   *
   * @details
   * Data of the row can be modified using the `set_row_data()` method
   *
   * @param parent, The TreeRow to be the parent of the created TreeRow
   *
   * @returns A new TreeRow from the table.
   */
  Gtk::TreeRow get_row(const Gtk::TreePath &path);

protected:
  static Glib::RefPtr<Gio::Icon> create_edit_button();

private:
  explicit EditColumnRecord(const std::shared_ptr<Gtk::TreeView> &view,
                            const std::vector<std::string> &names);

  Glib::RefPtr<Gtk::TreeStore> store;

  std::shared_ptr<Gtk::TreeView> view;

  Glib::RefPtr<Gtk::TreeModelFilter> filter_model;
  Glib::RefPtr<Gtk::TreeModelSort> sort_model;

  // Columns for the edit and delete buttons
  Gtk::TreeModelColumn<Glib::RefPtr<Gio::Icon>> edit_column;
  Gtk::TreeModelColumn<Glib::RefPtr<Gio::Icon>> delete_column;
};

#endif // TABS_EDIT_COLUMN_RECORD_H