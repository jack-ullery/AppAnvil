#include "entry_iter.h"
#include "../entries.h"
#include <gtkmm/treeiter.h>

template<class EntryType>
EntryIter<EntryType>::EntryIter(Gtk::TreeModel* model)
  : Gtk::TreeIter(model),
    entry()
{ }

template<class EntryType>
EntryIter<EntryType>::EntryIter(Gtk::TreeModel* model, EntryType entry)
  : Gtk::TreeIter(model),
    entry{entry}
{ }

template<class EntryType>
EntryIter<EntryType>::EntryIter(Gtk::TreeIter iter)
  : Gtk::TreeIter(iter)
{ }

template<class EntryType>
EntryType EntryIter<EntryType>::get_entry()
{
    return entry;
}

template class EntryIter<ProfileTableEntry>;
template class EntryIter<ProcessTableEntry>;
template class EntryIter<LogTableEntry>;