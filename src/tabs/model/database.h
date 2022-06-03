#ifndef TABS_MODEL_DATABASE_H
#define TABS_MODEL_DATABASE_H

#include <gtkmm/treeiter.h>
#include <map>
#include <memory>
#include <string>

#include "../column_header.h"
#include "../entries.h"
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
  std::map<std::string, ProcessTableEntry> process_data;
  std::map<std::string, LogTableEntry> log_data;

  friend class ProfileAdapter<Database>;
};

#endif // TABS_MODEL_DATABASE_H
