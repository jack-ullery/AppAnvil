#include "database.h"
#include "process_adapter.h"

#include <regex>
#include <stdexcept>

// clang-tidy throws [cert-err58-cpp], but it's not a problem in this case, so lets ignore it.
const std::regex unconfined_proc("^\\s*(\\S+)\\s+(\\S+)\\s+(\\S+)\\s+(unconfined|\\S+ \\(\\S+\\))\\s+(\\S+)"); // NOLINT(cert-err58-cpp)

template<class Database, class ColumnRecord>
Gtk::TreeRow ProcessAdapter<Database, ColumnRecord>::add_row(const std::string &process_name, 
                                                 const unsigned int &pid,  
                                                 const unsigned int &ppid, 
                                                 const std::string &user, 
                                                 const std::string &status)
{
    Gtk::TreeRow row;

    if(ppid > 0) {
        auto parent_row = col_record->get_parent_by_pid(ppid);
        row             = col_record->new_child_row(parent_row);
    } else {
        row = col_record->new_row();
    }

    row->set_value(0, process_name); // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    row->set_value(1, user);         // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    row->set_value(2, pid);          // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    row->set_value(3, status);       // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

    return row;
}

template<class Database, class ColumnRecord>
void ProcessAdapter<Database, ColumnRecord>::put_data(const std::string &process_name,
                                        const std::string &profile_name, 
                                        const unsigned int &pid,  
                                        const unsigned int &ppid, 
                                        const std::string &user, 
                                        const std::string &status)
{
    if(col_record == nullptr){
        throw std::runtime_error("Error: Attempted to write to ColumnRecord (Profile) before it was registered.");
    }

    // Attempt to find an entry with this profile name
    auto entry_pair  = get_data(process_name, pid);
    bool entry_found = entry_pair.second;

    if(entry_found){
        // A pre-existing entry was found, so we should modify it
        ProcessTableEntry entry = entry_pair.first;
        entry.process_name=process_name;
        entry.profile_name=profile_name;

        entry.row->set_value(1, user);         // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
        entry.row->set_value(3, status);       // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    }
    else {
        // If not entry was found, we should create one
        // create a new row
        auto row = ProcessAdapter<Database, ColumnRecord>::add_row(process_name, pid, ppid, user, status);

        ProcessTableEntry entry(process_name, profile_name, pid, row);

        // Assume no map of logs exists with this process_name, and attempt to add a new one 
        auto emplace_iter = db->process_data.emplace(profile_name, std::map<uint, ProcessTableEntry>());

        // Get the map that we just added (or the one that existed previously)
        std::map<uint, ProcessTableEntry> pid_map = emplace_iter.first->second;

        // Add the entry to the table
        pid_map.emplace(pid, entry);
    }
}

template<class Database, class ColumnRecord>
std::pair<ProcessTableEntry, bool> ProcessAdapter<Database, ColumnRecord>::get_data(std::string profile_name, const unsigned int &pid)
{
    auto pid_map_iter = db->process_data.find(profile_name);
    if(pid_map_iter != db->process_data.end()){
        auto pid_map = pid_map_iter->second;
        auto iter = pid_map.find(pid);
        if(iter != pid_map.end()){
            // We actually found some data, so return the found data
            return std::pair<ProcessTableEntry, bool>(iter->second, true);
        }
    }

    // If the entry is not found
    return std::pair<ProcessTableEntry, bool>(ProcessTableEntry(), false);
}

template<class Database, class ColumnRecord>
std::shared_ptr<ColumnRecord> ProcessAdapter<Database, ColumnRecord>::get_col_record() {
  return col_record;
}

template<class Database, class ColumnRecord>
ProcessAdapter<Database, ColumnRecord>::ProcessAdapter(std::shared_ptr<Database> db, 
                                         const std::shared_ptr<Gtk::TreeView> &view, 
                                         const std::shared_ptr<Gtk::ScrolledWindow> &win)
  : db{db},
    col_record{StatusColumnRecord::create(view, win, col_names)}  
{   }

template<class Database, class ColumnRecord>
LogAdapter<Database, ColumnRecord>::LogAdapter(std::shared_ptr<Database> db, std::shared_ptr<ColumnRecord> mock)
  : db{db},
    col_record{mock}  
{   }

template class ProcessAdapter<Database, StatusColumnRecord>;