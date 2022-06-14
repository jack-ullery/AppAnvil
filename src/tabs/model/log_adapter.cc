#include "database.h"
#include "log_adapter.h"
#include "status_column_record.h"

#include <bits/types/time_t.h>
#include <cstddef>
#include <gtest/gtest.h>
#include <gtkmm/treeiter.h>
#include <iomanip>
#include <regex>
#include <stdexcept>
#include <string>

template<class Database, class ColumnRecord>
std::string LogAdapter<Database, ColumnRecord>::format_log_data(std::string data)
{
  const std::regex remove_quotes = std::regex("\\\"(\\S*)\\\"");
  std::smatch m;
  std::regex_search(data, m, remove_quotes);
  return m[1];
}

template<class Database, class ColumnRecord>
std::string LogAdapter<Database, ColumnRecord>::format_timestamp(time_t timestamp)
{
  std::stringstream stream;

  auto *tm = localtime(&timestamp);
  stream << std::put_time(tm, "%F %T");

  return stream.str() + '\t';
}

template<class Database, class ColumnRecord>
void LogAdapter<Database, ColumnRecord>::put_data(const time_t &timestamp,
                                                  const std::string &type,
                                                  const std::string &operation,
                                                  const std::string &profile_name,
                                                  const unsigned int &pid,
                                                  const std::string &status)
{
    // Attempt to find an map with this profile name
    auto map_pair = db->log_data.find(profile_name);

    // The map (indexed by pid) that we will add to
    std::map<long, LogTableEntry> time_map;;

    // Check that we actually found the map
    if(map_pair == db->log_data.end()) {
      // Create new map if no previous one was found
      time_map = std::map<long, LogTableEntry>();
    }
    else {
      time_map = map_pair->second;
    }

    // Attempt to find an entry with this profile
    auto entry_pair = time_map.find((long) timestamp);

    // Check that we actually found the entry
    if(entry_pair != time_map.end()) {
        // A pre-existing entry was found, so we should return and not modify the db.
        // This assumes that logs entries are readonly; they do not change over time.
        return;
    }
    else {
        // If not entry was found, we should create one
        // create a new row
        const Gtk::TreeRow row = col_record->new_row();

        // clang-format off
        row.set_value(0, format_timestamp(timestamp)); // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
        row.set_value(1, format_log_data(type));       // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
        row.set_value(2, format_log_data(operation));  // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
        row.set_value(3, profile_name);                // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
        row.set_value(4, pid);                         // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
        row.set_value(5, format_log_data(status));     // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
        // clang-format on

        LogTableEntry entry(timestamp, type, operation, profile_name, pid, row);

        // Add the entry to the map
        time_map.insert({(long) timestamp, entry});
    }

    // A weird way of updating our profile in the map (because insert_or_assign does not exist with C++11)
    db->log_data.erase(profile_name);
    db->log_data.insert({profile_name, time_map});
}

template<class Database, class ColumnRecord>
std::pair<LogTableEntry, bool> LogAdapter<Database, ColumnRecord>::get_data(const std::string &profile_name, const time_t &timestamp)
{
    auto time_map_iter = db->log_data.find(profile_name);
    if(time_map_iter != db->log_data.end()){
        auto time_map = time_map_iter->second;
        auto iter = time_map.find(timestamp);
        if(iter != time_map.end()){
            // We actually found some data, so return the found data
            return std::pair<LogTableEntry, bool>(iter->second, true);
        }
    }

    // If the entry is not found
    return std::pair<LogTableEntry, bool>(LogTableEntry(), false);
}

template<class Database, class ColumnRecord>
std::shared_ptr<ColumnRecord> LogAdapter<Database, ColumnRecord>::get_col_record() {
  return col_record;
}

template<class Database, class ColumnRecord>
LogAdapter<Database, ColumnRecord>::LogAdapter(std::shared_ptr<Database> db, 
                                 const std::shared_ptr<Gtk::TreeView> &view, 
                                 const std::shared_ptr<Gtk::ScrolledWindow> &win)
  : db{db},
    col_record{ColumnRecord::create(view, win, col_names)}  
{   }

template<class Database, class ColumnRecord>
LogAdapter<Database, ColumnRecord>::LogAdapter(std::shared_ptr<Database> db, std::shared_ptr<ColumnRecord> mock)
  : db{db},
    col_record{mock}  
{   }

template class LogAdapter<Database, StatusColumnRecord>;