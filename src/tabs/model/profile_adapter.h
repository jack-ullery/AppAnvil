#ifndef TABS_MODEL_PROFILE_ADAPTER_H
#define TABS_MODEL_PROFILE_ADAPTER_H

#include <gtkmm/treeiter.h>
#include <map>
#include <memory>
#include <string>

#include "../column_header.h"
#include "../entries.h"
#include "database.h"
#include "status_column_record.h"

template<class Database> class ProfileAdapter
{
public:
  // Initializes the database adapter
  ProfileAdapter(std::shared_ptr<Database> db, const std::shared_ptr<Gtk::TreeView> &view, const std::shared_ptr<Gtk::ScrolledWindow> &win);

  void put_data(const std::string &profile_name, const std::string &status);
  ProfileTableEntry get_data(const std::string &profile_name);

  std::shared_ptr<StatusColumnRecord> get_col_record();

private:
  std::shared_ptr<Database> db;

  std::shared_ptr<StatusColumnRecord> col_record;
  const static std::vector<ColumnHeader> col_names;
};

template<class Database>
const std::vector<ColumnHeader> ProfileAdapter<Database>::col_names{ColumnHeader("Profile"), ColumnHeader("Status")};

#endif // TABS_MODEL_PROFILE_ADAPTER_H
