#ifndef TABS_ENTRY_STORE_H
#define TABS_ENTRY_STORE_H

#include "../column_header.h"
#include "../view/status.h"

#include <gtkmm/treestore.h>

class EntryStore : public Gtk::TreeStore
{
public:
  static Glib::RefPtr<EntryStore> create(const Gtk::TreeModelColumnRecord& columns);

protected:
 	explicit EntryStore(const Gtk::TreeModelColumnRecord& columns);

  virtual void set_value_impl(const Gtk::TreeModel::iterator& row, int column, const Glib::ValueBase & value) override;
  virtual void get_value_vfunc(const Gtk::TreeModel::iterator& row, int column, Glib::ValueBase &value) const override;
};

#endif // TABS_ENTRY_STORE_H