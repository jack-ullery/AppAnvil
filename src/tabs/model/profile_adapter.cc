#include "profile_adapter.h"
#include "database.h"

#include <stdexcept>

template<class Database>
void
ProfileAdapter<Database>::put_data(const std::string &profile_name, const std::string &status)
{
  if (col_record == nullptr) {
    throw std::runtime_error("Error: Attempted to write to ColumnRecord (Profile) before it was registered.");
  }

  // Attempt to find an entry with this profile name
  auto iter = db->profile_data.find(profile_name);

  if (iter == db->profile_data.end()) {
    // If no entry was found, we should create one
    ProfileTableEntry entry(profile_name, status);

    auto row = col_record->new_row(entry);
    row->set_value(0, profile_name);
    row->set_value(1, status);

    db->profile_data.insert({ profile_name, row });
  } else {
    // A pre-existing entry was found, so we should modify it
    EntryIter<ProfileTableEntry> row = iter->second;
    row.get_entry().status           = status;
    db->profile_data.erase(profile_name);
    db->profile_data.insert({ profile_name, row });
  }
}

template<class Database>
std::pair<ProfileTableEntry, bool>
ProfileAdapter<Database>::get_data(const std::string &profile_name)
{
  auto iter = db->profile_data.find(profile_name);

  if (iter == db->profile_data.end()) {
    // No data was found, so make up a fake entry
    return { ProfileTableEntry(profile_name, UNKNOWN_STR), false };
  }

  // Return the found entry
  return { iter->second.get_entry(), true };
}

template<class Database>
std::shared_ptr<StatusColumnRecord<ProfileTableEntry>>
ProfileAdapter<Database>::get_col_record()
{
  return col_record;
}

template<class Database>
uint
ProfileAdapter<Database>::get_number_processes(const std::string &profile)
{
  return db->get_number_processes(profile);
}

template<class Database>
uint
ProfileAdapter<Database>::get_number_logs(const std::string &profile)
{
  return db->get_number_logs(profile);
}

template<class Database>
ProfileAdapter<Database>::ProfileAdapter(std::shared_ptr<Database> db,
                                         const std::shared_ptr<Gtk::TreeView> &view,
                                         const std::shared_ptr<Gtk::ScrolledWindow> &win)
  : db{ db },
    col_record{ StatusColumnRecord<ProfileTableEntry>::create(view, win, col_names) }
{
}

template class ProfileAdapter<Database>;