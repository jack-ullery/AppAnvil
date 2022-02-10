#include "database.h"

void Database::add_profile_data(const ProfileTableEntry& entry)
{
  profile_data.emplace(entry.profile_name, entry);
}

void Database::add_process_data(const ProcessTableEntry& entry)
{
  process_data.emplace(entry.profile_name, entry);
}

void Database::add_log_data(const LogTableEntry& entry)
{
  log_data.emplace(entry.profile_name, entry);
}
