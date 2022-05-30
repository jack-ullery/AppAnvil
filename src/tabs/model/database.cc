#include "database.h"
#include <exception>
#include <gtkmm/treeiter.h>
#include <memory>
#include <stdexcept>

void Database::put_profile_data(const std::string profile_name, const std::string status)
{
    if(prof_col_record == nullptr){
        throw new std::runtime_error("Error: Attempted to write to ColumnRecord (Profile) before it was registered.");
    }

    // Attempt to find an entry with this profile name
    auto iter = profile_data.find(profile_name);

    if(iter == profile_data.end()){
        // If not entry was found, we should create one
        auto row = prof_col_record->new_row();
        row->set_value(0, profile_name);

        ProfileTableEntry entry(profile_name, status, row);
        profile_data.emplace(profile_name, entry);
    }
    else {
        // A pre-existing entry was found, so we should modify it
        ProfileTableEntry entry = iter->second;
        entry.status=status;
    }


}

void Database::put_process_data(const std::string profile_name, const unsigned int pid)
{
    // Not implemented yet!
}

void Database::put_log_data(const std::string profile_name, const unsigned int pid, const std::string timestamp, const std::string type, const std::string operation)
{
    // Not implemented yet!
}

void Database::register_profile_column_record(std::shared_ptr<StatusColumnRecord> col_record)
{
    if(col_record == nullptr) {
        throw new std::runtime_error("Error: Argument for ColumnRecord (Profile) is null.");
    } else if (prof_col_record != nullptr) {
        throw new std::runtime_error("Error: Attempted to initialize ColumnRecord (Profile) more than once.");
    }

    prof_col_record = col_record;
}

ProfileTableEntry Database::get_profile_data(const std::string &profile_name){
    auto iter = profile_data.find(profile_name);

    if(iter == profile_data.end()){
        // No data was found, so make up a fake entry
        return ProfileTableEntry(profile_name, UNKNOWN_STATUS, Gtk::TreeRow());;
    }

    // Return the found entry
    return iter->second;
}

Database::Database()
{   }