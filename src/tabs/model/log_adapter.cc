#include "database.h"
#include "log_adapter.h"
#include "status_column_record.h"

#include <cstddef>
#include <gtkmm/treeiter.h>
#include <iomanip>
#include <regex>
#include <stdexcept>

template<class Database, class ColumnRecord>
std::string LogAdapter<Database, ColumnRecord>::format_log_data(const std::string &data)
{
  const std::regex remove_quotes = std::regex("\\\"(\\S*)\\\"");
  std::smatch m;
  std::regex_search(data, m, remove_quotes);
  return m[1];
}

template<class Database, class ColumnRecord>
std::string LogAdapter<Database, ColumnRecord>::format_timestamp(const time_t &timestamp)
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
    if(col_record == nullptr){
        throw std::runtime_error("Error: Attempted to write to ColumnRecord (Profile) before it was registered.");
    }

    // Attempt to find an entry with this profile name
    auto entry_pair  = get_data(profile_name, timestamp);
    bool entry_found = entry_pair.second;

    if(entry_found){
        // A pre-existing entry was found, so we should modify it
        LogTableEntry entry = entry_pair.first;

        entry.type = type;
        entry.operation = operation;
        entry.profile_name = profile_name;

        // clang-format off
        entry.row.set_value(2, type);
        entry.row.set_value(3, operation);
        entry.row.set_value(4, profile_name);
        // clang-format on
    }
    else {
        // create a new row
        // auto row = col_record->new_row();
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

        // Assume no map of logs exists with this profile_name, and attempt to add a new one 
        auto emplace_iter = db->log_data.emplace(profile_name, std::map<time_t, LogTableEntry>());

        // Get the map that we just added (or the one that existed previously)
        std::map<time_t, LogTableEntry> time_map = emplace_iter.first->second;

        // Add the entry to the table
        time_map.emplace(timestamp, entry);
    }
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