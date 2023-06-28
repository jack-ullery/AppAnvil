#ifndef TABS_COMBOBOX_STORE_H
#define TABS_COMBOBOX_STORE_H

#include <gtkmm/liststore.h>
#include <gtkmm/treemodel.h>
#include <string>

class ComboboxStore
{
public:
  static Glib::RefPtr<Gtk::ListStore> create(std::initializer_list<std::pair<std::string, std::string>> values);

private:
  // Tree model columns for the Combo CellRenderer in the TreeView column:
  class ModelColumnsCombo : public Gtk::TreeModel::ColumnRecord
  {
  public:
    ModelColumnsCombo();
    Gtk::TreeModelColumn<Glib::ustring> m_col_choice;
    Gtk::TreeModelColumn<Glib::ustring> m_col_description;
  };
};

#endif // TABSCOMBO_BOX_STORE_H