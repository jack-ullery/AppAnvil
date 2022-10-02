#ifndef TABS_ENTRY_ITER_H
#define TABS_ENTRY_ITER_H

#include <gtkmm/treeiter.h>

template<class EntryType>
class EntryIter : public Gtk::TreeIter
{
public:
 	explicit EntryIter(Gtk::TreeModel* model);

	EntryType get_entry();

private:
	EntryType entry;
};

#endif // TABS_ENTRY_ITER_H