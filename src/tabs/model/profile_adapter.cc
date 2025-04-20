#include "profile_adapter.h"
#include "database.h"

#include <stdexcept>

template<class Database>
bool ProfileAdapter<Database>::put_data(const std::string &profile_name, const std::string &status)
{
  if (col_record == nullptr) {
    throw std::runtime_error("Error: Attempted to write to ColumnRecord (Profile) before it was registered.");
  }

  // Attempt to find an entry with this profile name
  auto iter = db->profile_data.find(profile_name);

  if (iter == db->profile_data.end()) {
    // If not entry was found, we should create one
    auto row = col_record->new_row();
    ProfileTableEntry entry(profile_name, status, row);

    row->set_value(0, entry);
    row->set_value(1, profile_name);
    row->set_value(2, status);

    db->profile_data.insert({ profile_name, entry });
  } else {
    // A pre-existing entry was found with a different status, we should modify it
    ProfileTableEntry entry = iter->second;
    if (entry.status != status) {
      entry.status = status;
      entry.row->set_value(2, status);
      db->profile_data.erase(profile_name);
      db->profile_data.insert({ profile_name, entry });
      return true;
    }
  }

  return false;
}

template<class Database>
std::pair<ProfileTableEntry, bool> ProfileAdapter<Database>::get_data(const std::string &profile_name)
{
  auto iter = db->profile_data.find(profile_name);

  if (iter == db->profile_data.end()) {
    // No data was found, so make up a fake entry
    return { ProfileTableEntry(profile_name, UNKNOWN_STR, Gtk::TreeRow()), false };
  }

  // Return the found entry
  return { iter->second, true };
}

template<class Database>
std::shared_ptr<StatusColumnRecord> ProfileAdapter<Database>::get_col_record()
{
  return col_record;
}

template<class Database>
uint ProfileAdapter<Database>::get_number_processes(const std::string &profile)
{
  return db->get_number_processes(profile);
}

template<class Database>
uint ProfileAdapter<Database>::get_number_logs(const std::string &profile)
{
  return db->get_number_logs(profile);
}

template<class Database>
void ProfileAdapter<Database>::set_profile_status_change_func(const StatusColumnRecord::change_function_type &fun)
{
  col_record->set_change_func(fun);
}

template<class Database>
ProfileAdapter<Database>::ProfileAdapter(std::shared_ptr<Database> db, const std::shared_ptr<Gtk::TreeView> &view)
  : db{ db },
    col_record{ StatusColumnRecord::create(view, col_names) }
{
}

template class ProfileAdapter<Database>;