#include "edit_column_record.h"
#include "../entries.h"

#include <glibmm/refptr.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/cellrendererpixbuf.h>
#include <gtkmm/cellrenderertext.h>
#include <gtkmm/image.h>
#include <gtkmm/main.h>
#include <gtkmm/treeiter.h>
#include <gtkmm/treemodelcolumn.h>
#include <gtkmm/treemodelsort.h>
#include <memory>
#include <sigc++/functors/ptr_fun.h>
#include <tuple>
#include <vector>

/*
    Public Methods
*/
std::shared_ptr<EditColumnRecord> EditColumnRecord::create(const std::shared_ptr<Gtk::TreeView> &view,
                                                           const std::vector<std::string> &names)
{
  std::shared_ptr<EditColumnRecord> record{ new EditColumnRecord(view, names) };

  auto store    = Gtk::TreeStore::create(*record);
  record->store = store;

  record->filter_model = Gtk::TreeModelFilter::create(store);
  record->sort_model   = Gtk::TreeModelSort::create(record->filter_model);
  view->set_model(record->sort_model);

  return record;
}

Gtk::TreeRow EditColumnRecord::new_row()
{
  auto row = *(store->append());

  row->set_value(1, create_edit_button());
  // row->set_value(2, create_edit_button());

  return row;
}

Gtk::TreeRow EditColumnRecord::get_row(const Gtk::TreePath &path)
{
  return *(sort_model->get_iter(path));
}

/*
    Private Methods
*/
Glib::RefPtr<Gio::Icon> EditColumnRecord::create_edit_button()
{
  auto button = Gio::Icon::create("edit-symbolic");
  return button;
}

EditColumnRecord::EditColumnRecord(const std::shared_ptr<Gtk::TreeView> &view,
                                   const std::vector<std::string> &names)
  : view{ view }
{
  for (uint i = 0; i < names.size(); i++) {
    // Add a visible column, and title it using the string from 'names'
    Gtk::TreeModelColumn<std::string> model_column;
    add(model_column);
    view->append_column(names[i], model_column);

    // Set some default settings for the columns
    // Note this a Gtk::TreeViewColumn which is different then the Gtk::TreeModelColumn which we use earlier
    auto *column_view = view->get_column(static_cast<int>(i));
    column_view->set_reorderable(false);
    column_view->set_resizable();
    column_view->set_min_width(MIN_COL_WIDTH);
    column_view->set_sort_column(model_column);
  }

  // Add and configure column for edit button
  add(edit_column);
  auto size = view->append_column("Edit", edit_column);

  auto *renderer = Gtk::make_managed<Gtk::CellRendererPixbuf>();
  auto *edit_view_column = view->get_column(size - 1);
  edit_view_column->clear();
  
  edit_view_column->pack_start(*renderer);
  edit_view_column->add_attribute(renderer->property_gicon(), edit_column);

  // // Add and configure column for delete button
  // add(delete_column);
  // view->append_column("Delete", delete_column);
}
