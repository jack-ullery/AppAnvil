#include "status_column_record.h"
#include "../entries.h"
#include "combobox_store.h"

#include <gtkmm.h>
#include <gtkmm/box.h>
#include <gtkmm/cellrenderertext.h>
#include <gtkmm/cellrenderertoggle.h>
#include <gtkmm/liststore.h>
#include <gtkmm/main.h>
#include <gtkmm/treeiter.h>
#include <gtkmm/treemodelcolumn.h>
#include <gtkmm/treemodelsort.h>
#include <iomanip>
#include <iostream>
#include <libappanvil/tree/AbstractionRule.hh>
#include <libappanvil/tree/FileRule.hh>
#include <memory>
#include <sigc++/functors/ptr_fun.h>
#include <stdexcept>
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

void StatusColumnRecord::set_change_func(const change_function_type &fun)
{
  change_fun = fun;
}

Gtk::TreeRow StatusColumnRecord::new_row()
{
  return *(store->append());
}

Gtk::TreeRow StatusColumnRecord::new_child_row(const Gtk::TreeRow &parent)
{
  return *(store->append(parent.children()));
}

Gtk::TreeModel::iterator StatusColumnRecord::get_iter(const Gtk::TreePath &path)
{
  return sort_model->get_iter(path);
}

Gtk::TreeModel::iterator StatusColumnRecord::get_iter(const Glib::ustring &path)
{
  return sort_model->get_iter(path);
}

Gtk::TreeRow StatusColumnRecord::get_row(const Gtk::TreePath &path)
{
  auto iter = get_iter(path);
  if (iter == nullptr) {
    throw std::runtime_error("get_iter() returned nullptr. This should not happen. If you see this, it is a bug...");
  }
  return *iter;
}

Gtk::TreeRow StatusColumnRecord::get_row(const Glib::ustring &path)
{
  auto iter = get_iter(path);
  if (iter == nullptr) {
    throw std::runtime_error("get_iter() returned nullptr. This should not happen. If you see this, it is a bug...");
  }
  return *iter;
}

void StatusColumnRecord::clear()
{
  store->clear();
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

StatusColumnRecord::StatusColumnRecord(const std::shared_ptr<Gtk::TreeView> &view, const std::vector<ColumnHeader> &names)
  : view{ view },
    filter_fun{ sigc::ptr_fun(&StatusColumnRecord::default_filter) },
    change_fun{ sigc::ptr_fun(&StatusColumnRecord::on_change) }
{
  view->set_activate_on_single_click(true);

  for (uint i = 0; i < names.size(); i++) {
    std::unique_ptr<Gtk::TreeModelColumnBase> column_base;

    switch (names[i].type) {
      case ColumnHeader::STRING: {
        // Add a visible column, and title it using the string from 'names'
        auto model_column = Gtk::TreeModelColumn<std::string>();
        add(model_column);
        view->append_column(names[i].name, model_column);
        column_base = std::make_unique<Gtk::TreeModelColumnBase>(model_column);
      } break;

      case ColumnHeader::BOOLEAN: {
        // Add a visible column, and title it using the string from 'names'
        auto model_column = Gtk::TreeModelColumn<bool>();
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

      case ColumnHeader::TIMESTAMP: {
        // Add a visible column, and title it using the string from 'names'
        auto model_column = Gtk::TreeModelColumn<unsigned int>();
        add(model_column);
        view->append_column(names[i].name, model_column);
        column_base = std::make_unique<Gtk::TreeModelColumnBase>(model_column);
      } break;

      case ColumnHeader::PROFILE_ENTRY: {
        // Add an invisible column, and title it using the string from 'names'
        auto model_column = Gtk::TreeModelColumn<ProfileTableEntry>();
        add(model_column);
        view->append_column(names[i].name, model_column);
        column_base = std::make_unique<Gtk::TreeModelColumnBase>(model_column);
      } break;

      case ColumnHeader::PROCESS_ENTRY: {
        // Add an invisible column, and title it using the string from 'names'
        auto model_column = Gtk::TreeModelColumn<ProcessTableEntry>();
        add(model_column);
        view->append_column(names[i].name, model_column);
        column_base = std::make_unique<Gtk::TreeModelColumnBase>(model_column);
      } break;

      case ColumnHeader::LOG_ENTRY: {
        // Add an invisible column, and title it using the string from 'names'
        auto model_column = Gtk::TreeModelColumn<LogTableEntry>();
        add(model_column);
        view->append_column(names[i].name, model_column);
        column_base = std::make_unique<Gtk::TreeModelColumnBase>(model_column);
      } break;

      case ColumnHeader::ABSTRACTION_RULE_POINTER: {
        // Add an invisible column, and title it using the string from 'names'
        auto model_column = Gtk::TreeModelColumn<std::shared_ptr<AppArmor::Tree::AbstractionRule>>();
        add(model_column);
        view->append_column(names[i].name, model_column);
        column_base = std::make_unique<Gtk::TreeModelColumnBase>(model_column);
      } break;

      case ColumnHeader::FILE_RULE_POINTER: {
        // Add an invisible column, and title it using the string from 'names'
        auto model_column = Gtk::TreeModelColumn<std::shared_ptr<AppArmor::Tree::FileRule>>();
        add(model_column);
        view->append_column(names[i].name, model_column);
        column_base = std::make_unique<Gtk::TreeModelColumnBase>(model_column);
      } break;

      case ColumnHeader::COMBO_BOX: {
        auto model_column = Gtk::TreeModelColumn<std::string>();
        add(model_column);
        view->append_column(names[i].name, model_column);
        column_base = std::make_unique<Gtk::TreeModelColumnBase>(model_column);
      } break;
    };

    // Set some default settings for the columns
    // Note this a Gtk::TreeViewColumn which is different then the Gtk::TreeModelColumn which we use earlier
    auto *column_view = view->get_column(static_cast<int>(i));

    if (column_view == nullptr) {
      std::cerr << "Could not create column (" << i << ") with header: " << names[i].name << std::endl;
      continue;
    }

    column_view->set_reorderable(false);
    column_view->set_resizable();
    column_view->set_min_width(MIN_COL_WIDTH);
    column_view->set_sort_column(*column_base);

    if (names[i].type == ColumnHeader::PROFILE_ENTRY || names[i].type == ColumnHeader::PROCESS_ENTRY ||
        names[i].type == ColumnHeader::LOG_ENTRY || names[i].type == ColumnHeader::FILE_RULE_POINTER ||
        names[i].type == ColumnHeader::ABSTRACTION_RULE_POINTER) {
      // Create a custom cell renderer which shows nothing for these entries
      auto *renderer    = Gtk::make_managed<Gtk::CellRendererText>();
      auto callback_fun = sigc::ptr_fun(&StatusColumnRecord::ignore_cell_render);

      column_view->clear();
      column_view->pack_start(*renderer, false);
      column_view->set_cell_data_func(*renderer, callback_fun);

      // Make this row invisible
      column_view->set_visible(false);
    } else if (names[i].type == ColumnHeader::BOOLEAN) {
      // Ensure that the togglebutton actually toggles when clicked
      auto *renderer = dynamic_cast<Gtk::CellRendererToggle *>(column_view->get_first_cell());

      if (renderer != nullptr) {
        renderer->set_activatable(true);
        renderer->set_sensitive(true);

        // Function that is caled when togglebutton is clicked
        auto lambda = [this, i](const std::string &path) -> void {
          auto iter = get_iter(path);

          bool value = false;
          iter->get_value(static_cast<int>(i), value);
          iter->set_value(static_cast<int>(i), !value);

          change_fun(path);
        };
        renderer->signal_toggled().connect(lambda);
      }
    } else if (names[i].type == ColumnHeader::COMBO_BOX) {
      auto *renderer = Gtk::make_managed<Gtk::CellRendererCombo>();

      column_view->clear();
      column_view->pack_start(*renderer);
      column_view->add_attribute(renderer->property_text(), *column_base);

      Glib::RefPtr<Gtk::ListStore> combobox_options = ComboboxStore::create(names[i].combobox_options);

      renderer->property_model()            = combobox_options;
      renderer->property_text_column()      = 0;
      renderer->property_editable()         = true;
      renderer->property_placeholder_text() = names[i].placeholder;

      // Called when a user changes the combobox
      auto lambda = [&, i](const Glib::ustring &path_string, const Glib::ustring &new_text) -> void {
        on_combobox_edited(path_string, new_text, static_cast<int>(i));
      };

      renderer->signal_edited().connect(lambda);

    } else if (names[i].type == ColumnHeader::TIMESTAMP) {
      // Ensure that the togglebutton actually toggles when clicked
      auto *renderer = Gtk::make_managed<Gtk::CellRendererText>();

      // Called when we want to render the timestamp
      auto render_lambda = [i](Gtk::CellRenderer *renderer, const Gtk::TreeIter &iter) -> void {
        auto *text_renderer = dynamic_cast<Gtk::CellRendererText *>(renderer);
        if (iter && text_renderer != nullptr) {
          unsigned int timestamp = 0;
          iter->get_value(static_cast<int>(i), timestamp);
          text_renderer->property_text() = format_timestamp(timestamp);
        }
      };

      column_view->clear();
      column_view->pack_start(*renderer, false);
      column_view->set_cell_data_func(*renderer, render_lambda);
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

void StatusColumnRecord::on_change(const std::string &node)
{
  std::ignore = node;
}

void StatusColumnRecord::on_combobox_edited(const Glib::ustring &path_string, const Glib::ustring &new_text, int col)
{
  Gtk::TreePath path(path_string);

  // Get the row from the path
  auto iter = get_iter(path);
  if (iter) {
    // Store the user's new text in the model
    iter->set_value(col, new_text);

    // Call the change_fun() because the row was manually edited
    this->change_fun(path_string);
  }
}

void StatusColumnRecord::ignore_cell_render(Gtk::CellRenderer *renderer, const Gtk::TreeIter &iter)
{
  std::ignore = iter;

  auto *text_renderer = dynamic_cast<Gtk::CellRendererText *>(renderer);
  if (text_renderer != nullptr) {
    text_renderer->property_text() = "";
  }
}

std::string StatusColumnRecord::format_timestamp(const time_t &timestamp, const std::locale &loc)
{
  if (timestamp == 0) {
    return "Unknown";
  }

  std::stringstream stream;

  std::tm bt{};
  auto *tm = localtime_r(&timestamp, &bt);
  stream.imbue(loc);
  stream << std::put_time(tm, "%c");

  return stream.str() + '\t';
}
