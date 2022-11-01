#include "log_adapter.h"
#include "database.h"
#include "status_column_record.h"

#include <bits/types/time_t.h>
#include <cstddef>
#include <gtkmm/treeiter.h>
#include <iomanip>
#include <regex>
#include <stdexcept>
#include <string>

template<class Database, class ColumnRecord>
std::string LogAdapter<Database, ColumnRecord>::format_timestamp(const time_t &timestamp)
{
  std::stringstream stream;

  std::tm bt{};
  auto *tm = localtime_r(&timestamp, &bt);
  stream << std::put_time(tm, "%F %T");

  return stream.str() + '\t';
}

template<class Database, class ColumnRecord>
void LogAdapter<Database, ColumnRecord>::put_data(const time_t &timestamp,
                                                  const std::string &type,
                                                  const std::string &operation,
                                                  const std::string &profile_name,
                                                  const unsigned int &pid,
                                                  const std::list<std::pair<std::string, std::string>> &metadata)
{
  // Attempt to find an map with this profile name
  auto num_log_iter = db->log_data.find(profile_name);

  // Attempt to find the number of logs already processed for this type of profile
  uint num_logs = 1;
  if (num_log_iter != db->log_data.end()) {
    num_logs = num_log_iter->second + 1;
  }

  // Create a new row
  const Gtk::TreeRow row = col_record->new_row();

  // If not entry was found, we should create a new one
  LogTableEntry entry(timestamp, profile_name, pid, metadata, row);

  // clang-format off
  row.set_value(0, entry);                       // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  row.set_value(1, format_timestamp(timestamp)); // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  row.set_value(2, type);                        // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  row.set_value(3, operation);                   // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  row.set_value(4, profile_name);                // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  row.set_value(5, pid);                         // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  // clang-format on

  // A weird way of updating our profile in the map (because insert_or_assign does not exist with C++11)
  db->log_data.erase(profile_name);
  db->log_data.insert({ profile_name, num_logs });
}

template<class Database, class ColumnRecord>
std::shared_ptr<ColumnRecord> LogAdapter<Database, ColumnRecord>::get_col_record()
{
  return col_record;
}

template<class Database, class ColumnRecord>
LogAdapter<Database, ColumnRecord>::LogAdapter(std::shared_ptr<Database> db,
                                               const std::shared_ptr<Gtk::TreeView> &view,
                                               const std::shared_ptr<Gtk::ScrolledWindow> &win)
  : db{ db },
    col_record{ ColumnRecord::create(view, win, col_names) }
{
}

template<class Database, class ColumnRecord>
LogAdapter<Database, ColumnRecord>::LogAdapter(std::shared_ptr<Database> db, std::shared_ptr<ColumnRecord> mock)
  : db{ db },
    col_record{ mock }
{
}

template class LogAdapter<Database, StatusColumnRecord>;