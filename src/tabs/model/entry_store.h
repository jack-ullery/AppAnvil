#ifndef TABS_ENTRY_STORE_H
#define TABS_ENTRY_STORE_H

#include "entry_row.h"

#include <glibmm/value.h>
#include <gtkmm/treeiter.h>
#include <gtkmm/treestore.h>
#include <map>
#include <unordered_map>
#include <vector>

class EntryStore : public Gtk::TreeStore // Might want to change to treemodel
{
public:
	static Glib::RefPtr<EntryStore> create(const Gtk::TreeModelColumnRecord& columns);
 	explicit EntryStore(const Gtk::TreeModelColumnRecord& columns);

protected:
	void set_value_impl(const Gtk::TreeIter& iter, int column, const Glib::ValueBase &value) override;
	// void get_value_impl(const const_iterator& iter, int column, Glib::ValueBase& value) const override; 

private:
	std::vector<std::unordered_map<int, Glib::ValueBase>> data;
};

#endif // TABS_ENTRY_STORE_H