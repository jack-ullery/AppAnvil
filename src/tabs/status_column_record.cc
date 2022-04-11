#include "status_column_record.h"

#include <gtkmm/box.h>
#include <gtkmm/main.h>
#include <gtkmm/treemodelcolumn.h>
#include <gtkmm/treemodelsort.h>
#include <memory>
#include <tuple>
#include <vector>

/*
    Public Methods
*/
std::shared_ptr<StatusColumnRecord> StatusColumnRecord::create(const std::shared_ptr<Gtk::TreeView> &view,
                                                               const std::shared_ptr<Gtk::ScrolledWindow> &win,
                                                               const std::vector<ColumnHeader> &names)
{
  std::shared_ptr<StatusColumnRecord> record{new StatusColumnRecord(view, win, names)};

  auto store    = Gtk::TreeStore::create(*record);
  record->store = store;

  record->filter_model = Gtk::TreeModelFilter::create(store);
  record->sort_model   = Gtk::TreeModelSort::create(record->filter_model);
  view->set_model(record->sort_model);

  return record;
}

void StatusColumnRecord::set_visible_func(const Gtk::TreeModelFilter::SlotVisible &filter)
{
  filter_fun = filter;
  filter_model->set_visible_func(filter);
}

Gtk::TreeRow StatusColumnRecord::new_row() { return *(store->append()); }

Gtk::TreeRow StatusColumnRecord::new_child_row(const Gtk::TreeRow &parent) { return *(store->append(parent.children())); }

void StatusColumnRecord::clear()
{
  // We want to remember the last selected row, so that when data is added, we can select it again if a similar row appears
  // If a future row has the same string values we treat it the same as this row
  significant_rows.clear();
  // Get the currently selected rows (right now the max is one)
  auto selection = view->get_selection()->get_selected_rows();

  // Add all selected rows to map
  for(const auto &path: selection) {
    bool isExpanded = view->row_expanded(path);
    RowData data(true, isExpanded);
    significant_rows.insert({path, data});
  }

  // Remember the path of every selected or expanded row in the TreeView
  auto children = store->children();
  remember_children_rows(children);

  // Remember last position of the ScrolledBar in the ScrolledWindow our TreeView is a member of
  remember_scrollbar_position();

  store->clear();
}

uint StatusColumnRecord::filter_rows()
{
  // Count the number of rows that are visible
  uint num_visible = 0;
  auto children    = store->children();

  for(auto row = children.begin(); row != children.end(); row++) {
    bool visible = filter_fun(row);

    if(visible) {
      num_visible++;
    }
  }

  // Refilter every row in table, deciding wheter they should be visible or not.
  filter_model->refilter();

  return num_visible;
}

Gtk::TreeRow StatusColumnRecord::get_parent_by_pid(unsigned int pid)
{
  Gtk::TreeRow parentRow;
  auto children = store->children();

  for(auto iter = children.begin(); iter != children.end(); iter++) {
    unsigned int row_pid = 0;
    auto row             = *iter;
    row.get_value(2, row_pid);
    if(row_pid == pid) {
      return row;
    }
    if(!row.children().empty() && pid_exists_in_child(pid, row)) {
      return get_parent_by_pid(pid, row);
    }
  }

  return parentRow;
}

Gtk::TreeRow StatusColumnRecord::get_parent_by_pid(unsigned int pid, const Gtk::TreeRow &parent)
{
  Gtk::TreeRow parentRow;
  auto children = parent->children();

  for(auto iter = children.begin(); iter != children.end(); iter++) {
    unsigned int row_pid = 0;
    auto row             = *iter;
    row.get_value(2, row_pid);
    if(row_pid == pid) {
      return row;
    }
    if(!row.children().empty() && pid_exists_in_child(pid, row)) {
      return get_parent_by_pid(pid, row);
    }
  }

  return parentRow;
}

bool StatusColumnRecord::pid_exists_in_child(unsigned int pid, const Gtk::TreeRow &parent)
{
  auto children = parent.children();

  for(auto iter = children.begin(); iter != children.end(); iter++) {
    unsigned int row_pid = 0;
    auto row             = *iter;
    row.get_value(2, row_pid);
    if(row_pid == pid || (!row.children().empty() && pid_exists_in_child(pid, row))) {
      return true;
    }
  }

  return false;
}

/*
    Private Methods
*/
StatusColumnRecord::StatusColumnRecord(const std::shared_ptr<Gtk::TreeView> &view, const std::shared_ptr<Gtk::ScrolledWindow> &win,
                                       const std::vector<ColumnHeader> &names)
    : view{view}, win{win}, filter_fun{sigc::ptr_fun(&StatusColumnRecord::default_filter)}
{
  for(uint i = 0; i < names.size(); i++) {
    std::unique_ptr<Gtk::TreeModelColumnBase> column_base;

    if(names[i].type == ColumnHeader::STRING) {
      // Add a visible column, and title it using the string from 'names'
      auto model_column = Gtk::TreeModelColumn<std::string>();
      add(model_column);
      view->append_column(names[i].name, model_column);
      column_base = std::make_unique<Gtk::TreeModelColumnBase>(model_column);
    } else {
      // Add a visible column, and title it using the string from 'names'
      auto model_column = Gtk::TreeModelColumn<unsigned int>();
      add(model_column);
      view->append_column(names[i].name, model_column);
      column_base = std::make_unique<Gtk::TreeModelColumnBase>(model_column);
    }

    // Set some default settings for the columns
    // Note this a Gtk::TreeViewColumn which is different then the Gtk::TreeModelColumn which we use earlier
    auto *column_view = view->get_column(i);
    column_view->set_reorderable();
    column_view->set_resizable();
    column_view->set_min_width(MIN_COL_WIDTH);
    column_view->set_sort_column(*column_base);
  }
}

void StatusColumnRecord::remember_scrollbar_position()
{
  // Remember last position of the ScrolledBar in the ScrolledWindow our TreeView is a member of
  last_vadjustment_value = win->get_vadjustment()->get_value();
  last_hadjustment_value = win->get_hadjustment()->get_value();
}

void StatusColumnRecord::reset_scrollbar_position()
{
  // Adjust the ScrolledWindow to its previous position
  while(Gtk::Main::events_pending()) {
    Gtk::Main::iteration();
  }

  win->get_vadjustment()->set_value(last_vadjustment_value);
  win->get_hadjustment()->set_value(last_hadjustment_value);
}

void StatusColumnRecord::remember_children_rows(const Gtk::TreeModel::Children &children)
{
  for(const auto &row: children) {
    // Get the path
    Gtk::TreePath path = store->get_path(row);
    // If the row is expanded, and not contained in the map
    bool isExpanded = view->row_expanded(path);
    bool exists     = significant_rows.find(path) != significant_rows.end();
    if(isExpanded && !exists) {
      // Add data to the map
      RowData data(false, isExpanded);
      significant_rows.insert({path, data});
    }

    // Do the same for this row's children
    remember_children_rows(row.children());
  }
}

void StatusColumnRecord::reselect_children_rows(const Gtk::TreeModel::Children &children)
{
  for(const auto &row: children) {
    // Get the path
    Gtk::TreePath path = store->get_path(row);

    // If this row has the same data as the previously selected row (when this ColumnRecord was last cleared), then select it
    auto row_pair = significant_rows.find(path);
    if(row_pair != significant_rows.end()) {
      auto row_data = row_pair->second;

      // If row was selected
      if(row_data.isSelected) {
        auto selection = view->get_selection();
        selection->select(path);
      }

      // If row was expanded
      if(row_data.isExpanded) {
        view->expand_to_path(path);
      }

      // Do the same for this row's children
      reselect_children_rows(row.children());
    }
  }
}

void StatusColumnRecord::reselect_rows()
{
  // Reselect/reexpand every row in the TreeView, if it was selected or expanded before
  auto children = store->children();
  reselect_children_rows(children);

  // Adjust the ScrolledWindow to its previous position
  reset_scrollbar_position();
}

bool StatusColumnRecord::default_filter(const Gtk::TreeModel::iterator &node)
{
  std::ignore = node;
  return true;
}