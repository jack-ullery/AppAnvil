#include "entry_store.h"
#include "../entries.h"
#include <glibmm/value.h>
#include <gtkmm/treeiter.h>
#include <gtkmm/treemodel.h>
#include <vector>

template<class EntryType>
Glib::RefPtr<EntryStore<EntryType>> EntryStore<EntryType>::create(const Gtk::TreeModelColumnRecord& columns){
    return Glib::RefPtr<EntryStore>(new EntryStore(columns));
}

template<class EntryType>
EntryStore<EntryType>::EntryStore(const Gtk::TreeModelColumnRecord& columns)
: Gtk::TreeStore(columns)
{ }

template<class EntryType>
EntryIter<EntryType> EntryStore<EntryType>::append(EntryType entry)
{
    EntryIter<EntryType> iter((Gtk::TreeModel*) this, entry);
    gtk_tree_store_append(gobj(), iter.gobj(), nullptr);
    return iter;
}

template<class EntryType>
EntryIter<EntryType> EntryStore<EntryType>::append(EntryType entry, const Gtk::TreeNodeChildren& node)
{
    EntryIter<EntryType> iter((Gtk::TreeModel*) this, entry);
    gtk_tree_store_append(gobj(), iter.gobj(), const_cast<GtkTreeIter*>(node.get_parent_gobject()));
    return iter;
}

template class EntryStore<ProfileTableEntry>;
template class EntryStore<ProcessTableEntry>;
template class EntryStore<LogTableEntry>;