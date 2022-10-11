#ifndef TABS_MODEL_DATABASE_H
#define TABS_MODEL_DATABASE_H

#include <gtkmm/treeiter.h>
#include <map>
#include <memory>
#include <string>

#include "../column_header.h"
#include "../entries.h"
#include "log_adapter.h"
#include "process_adapter.h"
#include "profile_adapter.h"
#include "status_column_record.h"

class Database
{
public:
  // Initializes the database
  Database() = default;

  // The number of processes at this profile
  uint get_number_processes(const std::string &profile);

  // The number of processes at this profile
  uint get_number_logs(const std::string &profile);

private:
  std::map<std::string, ProfileTableEntry> profile_data;

  // Indexed by profile, returns a map of procesess
  // Each process in the returned map is indexed by the pid it was added
  std::map<std::string, std::map<uint, ProcessTableEntry>> process_data;

  // Indexed by profile, returns the number of logs found
  std::map<std::string, uint> log_data;

  friend class ProfileAdapter<Database>;
  friend class ProcessAdapter<Database, StatusColumnRecord>;
  friend class LogAdapter<Database, StatusColumnRecord>;
};

#endif // TABS_MODEL_DATABASE_H
