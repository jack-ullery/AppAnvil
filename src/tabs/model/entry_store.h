#ifndef TABS_ENTRY_STORE_H
#define TABS_ENTRY_STORE_H

#include "entry_iter.cc"

#include <glibmm/value.h>
#include <gtkmm/treestore.h>

// A slight modification to Gtk::TreeStore to allow storing and using our custom EntryIter iterator class
// Most of the implemented code is lifted from: https://github.com/GNOME/gtkmm/blob/master/gtk/src/treestore.ccg
template<class EntryType>
class EntryStore : public Gtk::TreeStore
{
public:
	static Glib::RefPtr<EntryStore> create(const Gtk::TreeModelColumnRecord& columns);
 	explicit EntryStore(const Gtk::TreeModelColumnRecord& columns);

	EntryIter<EntryType> append(EntryType entry);
	EntryIter<EntryType> append(EntryType entry, const Gtk::TreeNodeChildren& node);
};

#endif // TABS_ENTRY_STORE_H