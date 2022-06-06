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

  // void put_process_data(const std::string &profile_name, const unsigned int &pid);
  // void put_log_data(const std::string &profile_name, const unsigned int &pid, const std::string &timestamp, const std::string &type, const std::string &operation);

private:
  std::map<std::string, ProfileTableEntry> profile_data;
  std::map<uint, ProcessTableEntry>        process_data;

  // Indexed by profile, returns a map of logs
  // Each log in the retrned map is indexed by the time it was added
  std::map<std::string, std::map<time_t, LogTableEntry>> log_data;

  friend class ProfileAdapter<Database>;
  friend class ProcessAdapter<Database>;
  friend class LogAdapter<Database>;
};

#endif // TABS_MODEL_DATABASE_H
