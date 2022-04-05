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

void StatusColumnRecord::clear() {
  // We want to remember the last selected row, so that when data is added, we can select it again if a similar row appears
  // If a future row has the same string values we treat it the same as this row
  significant_rows.clear();
  // Get the currently selected rows (right now the max is one)
  auto selection = view->get_selection()->get_selected_rows();

  // Add all selected rows to map
  for(auto path : selection){
    bool isExpanded = view->row_expanded(path);
    RowData data(true, isExpanded);
    significant_rows.insert({path, data});
  }

  // For each row in the TreeView
  auto children = store->children();
  for(auto row : children) {
    // Get the path
    Gtk::TreePath path = store->get_path(row);
    // If the row is expanded, and not contained in the map
    bool isExpanded = view->row_expanded(path);
    bool exists = significant_rows.find(path) != significant_rows.end();  
    if(isExpanded && !exists){
      // Add data to the map
      RowData data(false, isExpanded);
      significant_rows.insert({path, data});
    }
  }

  store->clear();  
}

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

/*
    Private Methods
*/
StatusColumnRecord::StatusColumnRecord(const std::shared_ptr<Gtk::TreeView> &view, const std::vector<ColumnHeader> &names)
    : view{std::move(view)},
      filter_fun{sigc::ptr_fun(&StatusColumnRecord::default_filter)}
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