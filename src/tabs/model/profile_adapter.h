#ifndef TABS_MODEL_PROFILE_ADAPTER_H
#define TABS_MODEL_PROFILE_ADAPTER_H

#include <gtkmm/treeiter.h>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "../column_header.h"
#include "../entries.h"
#include "status_column_record.h"

template<class Database>
class ProfileAdapter
{
public:
  // Initializes the database adapter
  ProfileAdapter(std::shared_ptr<Database> db, const std::shared_ptr<Gtk::TreeView> &view);

  // Inserts the tuple into the database
  // Returns true if any data was added
  // Returns false if this tuple already exists in the database
  bool put_data(const std::string &profile_name, const std::string &status);

  void set_profile_status_change_func(const StatusColumnRecord::change_function_type &fun);

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

  // clang-format off
  const std::vector<ColumnHeader> col_names {
    ColumnHeader("Metadata", ColumnHeader::ColumnType::PROFILE_ENTRY),
    ColumnHeader("Profile"),
    ColumnHeader("Status", "",
      {
        {"enforce", "Similar to a whitelist, will only allow actions that are granted by the profile."},
        {"complain", "Similar to a blacklist, will grant any permission that is not explicitly denied by a profile."},
        {"audit", "Equivalent to enforce mode, but all allowed or denied actions are logged"},
        {"disabled", "This profile exists, but will not actually function to confine processes."},

        //// Advanced (Should put in a seperate menu or something)
        // {"kill", "Equivalent to enforce mode, but kills any process that violates this profile"},
        // {"Mediate Deleted", ""},
        // {"Attach Disconnected", ""},
        // {"Chroot Relative", ""},
      }
    )
  };
  // clang-format on

  std::shared_ptr<StatusColumnRecord> col_record;
};

#endif // TABS_MODEL_PROFILE_ADAPTER_H
