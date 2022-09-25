#include "entry_store.h"
#include <gtkmm/treestore.h>
#include <memory>

Glib::RefPtr<EntryStore> EntryStore::create(const Gtk::TreeModelColumnRecord& columns)
{
  return Glib::RefPtr<EntryStore>(new EntryStore(columns));
}

void EntryStore::set_value_impl(const Gtk::TreeModel::iterator& row, int column, const Glib::ValueBase & value)
{
  Gtk::TreeStore::set_value_impl(row, column, value);
}

void EntryStore::get_value_vfunc(const Gtk::TreeModel::iterator& row, int column, Glib::ValueBase &value) const
{
  Gtk::TreeStore::get_value_vfunc(row, column, value);
}

EntryStore::EntryStore(const Gtk::TreeModelColumnRecord& columns)
: Gtk::TreeStore(columns)
{ }
