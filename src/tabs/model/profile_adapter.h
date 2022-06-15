#ifndef TABS_MODEL_PROFILE_ADAPTER_H
#define TABS_MODEL_PROFILE_ADAPTER_H

#include <gtkmm/treeiter.h>
#include <map>
#include <memory>
#include <string>

#include "../column_header.h"
#include "../entries.h"
#include "status_column_record.h"

template<class Database> class ProfileAdapter
{
public:
  // Initializes the database adapter
  ProfileAdapter(std::shared_ptr<Database> db, const std::shared_ptr<Gtk::TreeView> &view, const std::shared_ptr<Gtk::ScrolledWindow> &win);

  void put_data(const std::string &profile_name, const std::string &status);
  std::pair<ProfileTableEntry, bool> get_data(const std::string &profile_name);

  std::shared_ptr<StatusColumnRecord> get_col_record();

  // Number of processes with a profile
  // TODO(inherit): Need to clean up Adapter/DB, maybe make Adapter inherit DB somehow
  uint get_number_processes(const std::string &profile);

  // Number of processes related to a profile
  // TODO(inherit): Need to clean up Adapter/DB, maybe make Adapter inherit DB somehow
  uint get_number_logs(const std::string &profile);

private:
  std::shared_ptr<Database> db;

  const std::vector<ColumnHeader> col_names{ColumnHeader("Profile"), ColumnHeader("Status")};
  std::shared_ptr<StatusColumnRecord> col_record;
};

#endif // TABS_MODEL_PROFILE_ADAPTER_H
