#include "status_column_record.h"

#include <gtkmm/box.h>
#include <gtkmm/treemodelsort.h>
#include <tuple>
#include <vector>

/*
    Public Methods
*/
std::shared_ptr<StatusColumnRecord> StatusColumnRecord::create(const std::shared_ptr<Gtk::TreeView> &view, std::vector<std::string> names)
{

  std::shared_ptr<StatusColumnRecord> record{new StatusColumnRecord(names)};

  auto store    = Gtk::TreeStore::create(*record);
  record->store = store;

  record->model_filter = Gtk::TreeModelFilter::create(store);
  auto sort_model      = Gtk::TreeModelSort::create(record->model_filter);
  view->set_model(sort_model);

  for(uint i = 0; i < names.size(); i++) {
    // Add a visible column, and title it using the string from 'names'
    view->append_column(names[i], record->column[i]);
    // Set some default settings for the columns
    // Note this a Gtk::TreeViewColumn which is different then the Gtk::TreeModelColumn we have as a field in StatusColumnRecord
    auto *column = view->get_column(i);
    column->set_reorderable();
    column->set_resizable();
    column->set_min_width(MIN_COL_WIDTH);
    column->set_sort_column(record->column[i]);
  }

  return record;
}

void StatusColumnRecord::set_visible_func(const Gtk::TreeModelFilter::SlotVisible &filter)
{
  filter_fun = filter;
  model_filter->set_visible_func(filter);
}

Gtk::TreeRow StatusColumnRecord::new_row() { return *(store->append()); }

Gtk::TreeRow StatusColumnRecord::new_child_row(const Gtk::TreeRow &parent) { return *(store->append(parent.children())); }

// Can delete method: look into `TreeRow::set_value`
void StatusColumnRecord::set_row_data(const Gtk::TreeRow &row, const uint &index, const std::string &data)
{
  if(index >= column.size()) {
    throw std::out_of_range("Attempting to access invalid column.");
  }

  row[this->column[index]] = data;
}

std::string StatusColumnRecord::get_row_data(const Gtk::TreeRow &row, const uint &index)
{
  if(index >= column.size()) {
    throw std::out_of_range("Attempting to access invalid column.");
  }

  return row[this->column[index]];
}

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

/*
    Private Methods
*/
StatusColumnRecord::StatusColumnRecord(const std::vector<std::string> &names)
    : filter_fun{sigc::ptr_fun(&StatusColumnRecord::default_filter)}
{
  this->column = std::vector<Gtk::TreeModelColumn<std::string>>(names.size());

  for(uint i = 0; i < column.size(); i++) {
    column[i] = Gtk::TreeModelColumn<std::string>();
    add(column[i]);
  }
}

bool StatusColumnRecord::default_filter(const Gtk::TreeModel::iterator &node)
{
  std::ignore = node;
  return true;
}