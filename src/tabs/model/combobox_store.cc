#include "combobox_store.h"

Glib::RefPtr<Gtk::ListStore> ComboboxStore::create(std::initializer_list<std::pair<std::string, std::string>> values)
{
  ModelColumnsCombo model;
  auto store = Gtk::ListStore::create(model);

  for(auto pair : values) {
    auto row = *store->append();
    row[model.m_col_choice] = pair.first;
    row[model.m_col_description] = pair.second;
  }

  return store;
}

ComboboxStore::ModelColumnsCombo::ModelColumnsCombo()
{
  add(m_col_choice);
  add(m_col_description);
}
