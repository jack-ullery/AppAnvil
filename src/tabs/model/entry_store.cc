#include "entry_store.h"
#include "entry_row.h"
#include <glibmm/value.h>
#include <gtkmm/treeiter.h>
#include <gtkmm/treemodel.h>
#include <vector>

Glib::RefPtr<EntryStore> EntryStore::create(const Gtk::TreeModelColumnRecord& columns)
{
    return Glib::RefPtr<EntryStore>(new EntryStore(columns));
}

EntryStore::EntryStore(const Gtk::TreeModelColumnRecord& columns)
: Gtk::TreeStore(columns),
  data()
{ }

void EntryStore::set_value_impl(const Gtk::TreeIter& iter, int column, const Glib::ValueBase &value)
{
    // // Set the stamp to be the next unoccupied index in the vector
    // uint stamp = iter->get_stamp();

    // if(stamp < data.size()) {
    //     auto entry_map = data.at(stamp);
    //     entry_map.erase(column);
    //     entry_map.emplace(column, value);
    // }

    // Lifted from the implementation of TreeStore
    gtk_tree_store_set_value(
      gobj(), const_cast<GtkTreeIter*>(iter.gobj()),
      column, const_cast<GValue*>(value.gobj()));

    // Signal to the parent that the row at this path has been changed
    // auto path = get_path(iter);
    // row_changed(path, iter);
}
