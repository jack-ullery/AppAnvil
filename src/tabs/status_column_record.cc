#include "status_column_record.h"

#include <gtkmm/box.h>
#include <gtkmm/treeiter.h>
#include <gtkmm/treemodelsort.h>
#include <memory>
#include <tuple>
#include <vector>

/*
    Public Methods
*/
std::shared_ptr<StatusColumnRecord> StatusColumnRecord::create(const std::shared_ptr<Gtk::TreeView>& view, std::vector<std::string> names)
{
  std::shared_ptr<StatusColumnRecord> record{new StatusColumnRecord(names)};

  auto store = Gtk::TreeStore::create(*record);
  record->store = store;
  record->view = view;

  record->filter_model = Gtk::TreeModelFilter::create(store);
  record->sort_model = Gtk::TreeModelSort::create(record->filter_model);
  view->set_model(record->sort_model);

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

void StatusColumnRecord::set_visible_func(const Gtk::TreeModelFilter::SlotVisible& filter)
{
  filter_fun = filter;
  filter_model->set_visible_func(filter);
}

Gtk::TreeRow StatusColumnRecord::new_row()
{
  return *(store->append());
}

Gtk::TreeRow StatusColumnRecord::new_child_row(const Gtk::TreeRow& parent)
{
  return *(store->append(parent.children()));
}

std::string StatusColumnRecord::get_row_data(const Gtk::TreeRow& row, const uint& index)
{
  if(index >= column.size()) {
    throw std::out_of_range("Attempting to access invalid column.");
  }

  return row[this->column[index]];
}

std::string to_string(std::vector<std::string> vec)
{
  if(vec.empty()) {
    return "NOTHING";
  }

  return vec[0];
}


void StatusColumnRecord::clear()
{
  // We want to remember the last selected row, so that when data is added, we can select it again if a similar row appears
  // We represent the last selected row as a vector of strings
  // If a future row has the same string values we treat it the same as this row
  this->last_selected_row.clear();
  // Get the currently selection, if there is only one row selected, then we can remember the last one
  // Multi-row selections are currently not supported
  auto selection = this->view->get_selection();

  if(selection->count_selected_rows() == 1) {
    // Get the selected row
    auto row = *selection->get_selected();
    // Get each value from each column and push it to the vector
    this->last_selected_row = row_to_vector(row);
  }

  // Finally we can clear all the data from the liststore
  store->clear();
}

uint StatusColumnRecord::filter_rows()
{
  // Refilter every row in table, deciding wheter they should be visible or not.
  filter_model->refilter();
  // If there has been no row selected, and clear() has been called previously, reselect the row that was selected before clear() was called
  reselect_row();

  // Count the number of rows that are visible
  uint num_visible = 0;
  auto children = store->children();

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
StatusColumnRecord::StatusColumnRecord(const std::vector<std::string>& names)
  : filter_fun{sigc::ptr_fun(&StatusColumnRecord::default_filter)}
{
  this->column = std::vector<Gtk::TreeModelColumn<std::string>>(names.size());

  for(uint i = 0; i < column.size(); i++) {
    column[i] = Gtk::TreeModelColumn<std::string>();
    add(column[i]);
  }
}

std::vector<std::string> StatusColumnRecord::row_to_vector(const Gtk::TreeRow& row)
{
  std::vector<std::string> vec;

  for(uint i = 0; i < this->column.size(); i++) {
    std::string str = get_row_data(row, i);
    vec.push_back(str);
  }

  return vec;
}

void StatusColumnRecord::reselect_row()
{
  // If there is no row currently selected, reselect the row from the previously
  if(this->view->get_selection()->count_selected_rows() == 0) {
    auto children = sort_model->children();

    for(auto iter = children.begin(), end = children.end(); iter != end; iter++) {
      // Get the vectorization of this row
      std::vector<std::string> row_vec = row_to_vector(*iter);

      // If this row has the same data as the previously selected row (when this ColumnRecord was last cleared), then select it
      if(last_selected_row == row_vec) {
        auto selection = view->get_selection();
        selection->select(iter);
      }
    }
  }
}

bool StatusColumnRecord::default_filter(const Gtk::TreeModel::iterator& node)
{
  std::ignore = node;
  return true;
}