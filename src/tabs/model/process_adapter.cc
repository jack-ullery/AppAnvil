#include "process_adapter.h"
#include "database.h"

#include <regex>
#include <stdexcept>

template<class Database, class ColumnRecord>
ProcessTableEntry ProcessAdapter<Database, ColumnRecord>::add_row(const std::string &profile_name,
                                                                  const std::string &process_name,
                                                                  const unsigned int &pid,
                                                                  const unsigned int &ppid,
                                                                  const std::string &user,
                                                                  const std::string &status)
{
  Gtk::TreeRow row;

  if (ppid > 0) {
    auto parent_row = col_record->get_parent_by_pid(ppid);
    row             = col_record->new_child_row(parent_row);
  } else {
    row = col_record->new_row();
  }

  ProcessTableEntry entry(process_name, profile_name, pid, row);

  row->set_value(0, entry);        // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  row->set_value(1, process_name); // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  row->set_value(2, user);         // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  row->set_value(3, pid);          // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  row->set_value(4, status);       // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

  return entry;
}

template<class Database, class ColumnRecord>
void ProcessAdapter<Database, ColumnRecord>::put_data(const std::string &process_name,
                                                      const std::string &profile_name,
                                                      const unsigned int &pid,
                                                      const unsigned int &ppid,
                                                      const std::string &user,
                                                      const std::string &status)
{
  // Attempt to find an map with this profile name
  auto map_pair = db->process_data.find(profile_name);

  // The map (indexed by pid) that we will add to
  std::map<uint, ProcessTableEntry> pid_map;

  // Check that we actually found the map
  if (map_pair == db->process_data.end()) {
    // Create new map if no previous one was found
    pid_map = std::map<uint, ProcessTableEntry>();
  } else {
    pid_map = map_pair->second;
  }

  // Attempt to find an entry with this profile
  auto entry_pair = pid_map.find(pid);

  // Check that we actually found the entry
  if (entry_pair != pid_map.end()) {
    // A pre-existing entry was found, so we should modify it
    ProcessTableEntry entry = entry_pair->second;
    entry.process_name      = process_name;
    entry.profile_name      = profile_name;

    entry.row->set_value(2, user);   // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    entry.row->set_value(4, status); // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

    // Add the entry to the map
    pid_map.erase(pid);
    pid_map.insert({ pid, entry });
  } else {
    // If no entry was found, we should create one
    auto entry = ProcessAdapter<Database, ColumnRecord>::add_row(profile_name, process_name, pid, ppid, user, status);

    // Add the entry to the map
    pid_map.insert({ pid, entry });
  }

  // A weird way of updating our profile in the map (because insert_or_assign does not exist with C++11)
  db->process_data.erase(profile_name);
  db->process_data.insert({ profile_name, pid_map });
}

template<class Database, class ColumnRecord>
std::pair<ProcessTableEntry, bool> ProcessAdapter<Database, ColumnRecord>::get_data(const std::string &profile_name,
                                                                                    const unsigned int &pid)
{
  auto pid_map_iter = db->process_data.find(profile_name);
  if (pid_map_iter != db->process_data.end()) {
    auto pid_map = pid_map_iter->second;
    auto iter    = pid_map.find(pid);
    if (iter != pid_map.end()) {
      // We actually found some data, so return the found data
      return std::pair<ProcessTableEntry, bool>(iter->second, true);
    }
  }

  // If the entry is not found
  return std::pair<ProcessTableEntry, bool>(ProcessTableEntry(), false);
}

template<class Database, class ColumnRecord>
std::shared_ptr<ColumnRecord> ProcessAdapter<Database, ColumnRecord>::get_col_record()
{
  return col_record;
}

template<class Database, class ColumnRecord>
ProcessAdapter<Database, ColumnRecord>::ProcessAdapter(std::shared_ptr<Database> db, const std::shared_ptr<Gtk::TreeView> &view)
  : db{ db },
    col_record{ StatusColumnRecord::create(view, col_names) }
{
}

template<class Database, class ColumnRecord>
ProcessAdapter<Database, ColumnRecord>::ProcessAdapter(std::shared_ptr<Database> db, std::shared_ptr<ColumnRecord> mock)
  : db{ db },
    col_record{ mock }
{
}

template class ProcessAdapter<Database, StatusColumnRecord>;