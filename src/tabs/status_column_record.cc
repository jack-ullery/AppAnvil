#include "status_column_record.h"

#include <gtkmm/box.h>
#include <gtkmm/treemodelcolumn.h>
#include <gtkmm/treemodelsort.h>
#include <memory>
#include <tuple>
#include <vector>

/*
    Public Methods
*/
std::shared_ptr<StatusColumnRecord> StatusColumnRecord::create(const std::shared_ptr<Gtk::TreeView> &view,
                                                               const std::vector<ColumnHeader> &names)
{
  std::shared_ptr<StatusColumnRecord> record{new StatusColumnRecord(view, names)};

  auto store    = Gtk::TreeStore::create(*record);
  record->store = store;

  record->model_filter = Gtk::TreeModelFilter::create(store);
  auto sort_model      = Gtk::TreeModelSort::create(record->model_filter);
  view->set_model(sort_model);

  return record;
}

void StatusColumnRecord::set_visible_func(const Gtk::TreeModelFilter::SlotVisible &filter)
{
  filter_fun = filter;
  model_filter->set_visible_func(filter);
}

Gtk::TreeRow StatusColumnRecord::new_row() { return *(store->append()); }

Gtk::TreeRow StatusColumnRecord::new_child_row(const Gtk::TreeRow &parent) { return *(store->append(parent.children())); }

void StatusColumnRecord::clear() { store->clear(); }

uint StatusColumnRecord::filter_rows()
{
  // Refilter every row in table, deciding wheter they should be visible or not.
  model_filter->refilter();

  // Count the number of rows that are visible
  uint num_visible = 0;
  auto children    = store->children();

  for(auto row = children.begin(); row != children.end(); row++) {
    bool visible = filter_fun(row);

    if(visible) {
      num_visible++;
    }
  }

  return num_visible;
}

Gtk::TreeRow StatusColumnRecord::get_parent_by_pid(unsigned int pid)
{
  Gtk::TreeRow parentRow;
  auto children = store->children();

  for(auto iter = children.begin(); iter != children.end(); iter++) {
    unsigned int row_pid = 0;
    auto row = *iter;
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
    auto row = *iter;
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
    unsigned int row_pid  = 0;
    auto row = *iter;
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
StatusColumnRecord::StatusColumnRecord(const std::shared_ptr<Gtk::TreeView> &view, const std::vector<ColumnHeader> &names)
    : filter_fun{sigc::ptr_fun(&StatusColumnRecord::default_filter)}
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

bool StatusColumnRecord::default_filter(const Gtk::TreeModel::iterator &node)
{
  std::ignore = node;
  return true;
}