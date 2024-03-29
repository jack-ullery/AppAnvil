#include "database.h"

uint Database::get_number_processes(const std::string &profile)
{
  auto pid_map = process_data.find(profile);

  if (pid_map == process_data.end()) {
    // No data was found, so there are no active processes for this profile
    return 0;
  }

  return pid_map->second.size();
}

uint Database::get_number_logs(const std::string &profile)
{
  auto num_log_iter = log_data.find(profile);

  if (num_log_iter == log_data.end()) {
    // No data was found, so there are no active processes for this profile
    return 0;
  }

  return num_log_iter->second;
}