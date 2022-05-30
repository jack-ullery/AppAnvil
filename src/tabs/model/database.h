#ifndef TABS_MODEL_DATABASE_H
#define TABS_MODEL_DATABASE_H

#include <gtkmm/treeiter.h>
#include <map>
#include <memory>
#include <string>

#include "../column_header.h"
#include "../entries.h"
#include "status_column_record.h"

class Database
{
public:
  // Initializes the database
  Database();

  void register_profile_column_record(std::shared_ptr<StatusColumnRecord> col_record);

  void put_profile_data(const std::string profile_name, const std::string status);
  void put_process_data(const std::string profile_name, const unsigned int pid);
  void put_log_data(const std::string profile_name, const unsigned int pid, const std::string timestamp, const std::string type, const std::string operation);

  ProfileTableEntry get_profile_data(const std::string &profile_name);

private:
  std::map<std::string, ProfileTableEntry> profile_data;
  std::map<std::string, ProcessTableEntry> process_data;
  std::map<std::string, LogTableEntry> log_data;

  std::shared_ptr<StatusColumnRecord> prof_col_record = NULL;
};

#endif // TABS_MODEL_DATABASE_H
