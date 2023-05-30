#include "status_column_record.h"
#include "../entries.h"

#include <gtkmm/box.h>
#include <gtkmm/cellrenderertext.h>
#include <gtkmm/main.h>
#include <gtkmm/treeiter.h>
#include <gtkmm/treemodelcolumn.h>
#include <gtkmm/treemodelsort.h>
#include <iostream>
#include <memory>
#include <sigc++/functors/ptr_fun.h>
#include <tuple>
#include <vector>

/*
    Public Methods
*/
std::shared_ptr<StatusColumnRecord> StatusColumnRecord::create(const std::shared_ptr<Gtk::TreeView> &view,
                                                               const std::vector<ColumnHeader> &names)
{
  std::shared_ptr<StatusColumnRecord> record{ new StatusColumnRecord(view, names) };

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

Gtk::TreeRow StatusColumnRecord::new_row()
{
  return *(store->append());
}

Gtk::TreeRow StatusColumnRecord::new_child_row(const Gtk::TreeRow &parent)
{
  return *(store->append(parent.children()));
}

Gtk::TreeRow StatusColumnRecord::get_row(const Gtk::TreePath &path)
{
  return *(sort_model->get_iter(path));
}

uint StatusColumnRecord::filter_rows()
{
  // Count the number of rows that are visible
  uint num_visible = 0;
  auto children    = store->children();

  for (auto row = children.begin(); row != children.end(); row++) {
    bool visible = filter_fun(row);

    if (visible) {
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

  for (auto iter = children.begin(); iter != children.end(); iter++) {
    unsigned int row_pid = 0;
    auto row             = *iter;
    row.get_value(3, row_pid);
    if (row_pid == pid) {
      return row;
    }
    if (!row.children().empty() && pid_exists_in_child(pid, row)) {
      return get_parent_by_pid(pid, row);
    }
  }

  return parentRow;
}

Gtk::TreeRow StatusColumnRecord::get_parent_by_pid(unsigned int pid, const Gtk::TreeRow &parent)
{
  Gtk::TreeRow parentRow;
  auto children = parent->children();

  for (auto iter = children.begin(); iter != children.end(); iter++) {
    unsigned int row_pid = 0;
    auto row             = *iter;
    row.get_value(3, row_pid);
    if (row_pid == pid) {
      return row;
    }
    if (!row.children().empty() && pid_exists_in_child(pid, row)) {
      return get_parent_by_pid(pid, row);
    }
  }

  return parentRow;
}

bool StatusColumnRecord::pid_exists_in_child(unsigned int pid, const Gtk::TreeRow &parent)
{
  auto children = parent.children();

  for (auto iter = children.begin(); iter != children.end(); iter++) {
    unsigned int row_pid    = 0;
    const Gtk::TreeRow &row = *iter;

    row.get_value(3, row_pid);
    if (row_pid == pid || (!row.children().empty() && pid_exists_in_child(pid, row))) {
      return true;
    }
  }

  return false;
}

/*
    Protected Methods
*/
StatusColumnRecord::StatusColumnRecord(const std::shared_ptr<Status> &tab, const std::vector<ColumnHeader> &names)
  : StatusColumnRecord(tab->get_view(), names)
{
}

StatusColumnRecord::StatusColumnRecord(const std::shared_ptr<Gtk::TreeView> &view,
                                       const std::vector<ColumnHeader> &names)
  : view{ view },
    filter_fun{ sigc::ptr_fun(&StatusColumnRecord::default_filter) }
{
  for (uint i = 0; i < names.size(); i++) {
    std::unique_ptr<Gtk::TreeModelColumnBase> column_base;

    switch (names[i].type) {
      default:
      case ColumnHeader::STRING: {
        // Add a visible column, and title it using the string from 'names'
        auto model_column = Gtk::TreeModelColumn<std::string>();
        add(model_column);
        view->append_column(names[i].name, model_column);
        column_base = std::make_unique<Gtk::TreeModelColumnBase>(model_column);
      } break;

      case ColumnHeader::INT: {
        // Add a visible column, and title it using the string from 'names'
        auto model_column = Gtk::TreeModelColumn<unsigned int>();
        add(model_column);
        view->append_column(names[i].name, model_column);
        column_base = std::make_unique<Gtk::TreeModelColumnBase>(model_column);
      } break;

      case ColumnHeader::PROFILE_ENTRY: {
        // Add a visible column, and title it using the string from 'names'
        auto model_column = Gtk::TreeModelColumn<ProfileTableEntry>();
        add(model_column);
        view->append_column(names[i].name, model_column);
        column_base = std::make_unique<Gtk::TreeModelColumnBase>(model_column);
      } break;

      case ColumnHeader::PROCESS_ENTRY: {
        // Add a visible column, and title it using the string from 'names'
        auto model_column = Gtk::TreeModelColumn<ProcessTableEntry>();
        add(model_column);
        view->append_column(names[i].name, model_column);
        column_base = std::make_unique<Gtk::TreeModelColumnBase>(model_column);
      } break;

      case ColumnHeader::LOG_ENTRY: {
        // Add a visible column, and title it using the string from 'names'
        auto model_column = Gtk::TreeModelColumn<LogTableEntry>();
        add(model_column);
        view->append_column(names[i].name, model_column);
        column_base = std::make_unique<Gtk::TreeModelColumnBase>(model_column);
      } break;
    };

    // Set some default settings for the columns
    // Note this a Gtk::TreeViewColumn which is different then the Gtk::TreeModelColumn which we use earlier
    auto *column_view = view->get_column(static_cast<int>(i));
    if(column_view != nullptr) {
      column_view->set_reorderable(false);
      column_view->set_resizable();
      column_view->set_min_width(MIN_COL_WIDTH);
      column_view->set_sort_column(*column_base);

      if (names[i].type == ColumnHeader::PROFILE_ENTRY || names[i].type == ColumnHeader::PROCESS_ENTRY ||
          names[i].type == ColumnHeader::LOG_ENTRY) {
        // Create a custom cell renderer which shows nothing for these entries
        auto *renderer    = Gtk::make_managed<Gtk::CellRendererText>();
        auto callback_fun = sigc::ptr_fun(&StatusColumnRecord::ignore_cell_render);

        column_view->clear();
        column_view->pack_start(*renderer, false);
        column_view->set_cell_data_func(*renderer, callback_fun);

        // Make this row invisible
        column_view->set_visible(false);
      }
    }
    else {
      std::cerr << "Could not create column (" << i << ") with header: " << names[i].name << std::endl;
    }
  }
}

/*
    Private Methods
*/
bool StatusColumnRecord::default_filter(const Gtk::TreeModel::iterator &node)
{
  std::ignore = node;
  return true;
}

void StatusColumnRecord::ignore_cell_render(Gtk::CellRenderer *renderer, const Gtk::TreeIter &iter)
{
  std::ignore = iter;

  auto *text_renderer = dynamic_cast<Gtk::CellRendererText *>(renderer);
  if (text_renderer != nullptr) {
    text_renderer->property_text() = "";
  }
}
