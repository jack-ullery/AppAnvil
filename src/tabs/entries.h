#ifndef TABS_ENTRIES_H
#define TABS_ENTRIES_H

#include <gtkmm/treeiter.h>
#include <string>

constexpr auto UNKNOWN_STR    = "unknown";
constexpr uint UNKNOWN_UINT   = -1;
constexpr time_t UNKNOWN_TIME = -1;

struct ProfileTableEntry
{
  std::string profile_name; // Unique, Primary Key
  std::string status;
  Gtk::TreeRow row;

  ProfileTableEntry(const std::string &_profile_name, const std::string &_status, const Gtk::TreeRow &_row)
    : profile_name{ _profile_name },
      status{ _status },
      row{ _row }
  {
  }

  // Default constructor, used when no entry found
  ProfileTableEntry()
    : profile_name{ UNKNOWN_STR },
      status{ UNKNOWN_STR },
      row()
  {
  }
};

struct ProcessTableEntry
{
  std::string process_name;
  std::string profile_name; // Refrences profile_name from ProfileTabEntry (if not null)
  unsigned int pid;         // Unique, Primary Key
  Gtk::TreeRow row;

  ProcessTableEntry(const std::string &_process_name, const std::string &_profile_name, const unsigned int &_pid, const Gtk::TreeRow &_row)
    : process_name{ _process_name },
      profile_name{ _profile_name },
      pid{ _pid },
      row{ _row }
  {
  }

  // Default constructor, used when no entry found
  ProcessTableEntry()
    : profile_name{ UNKNOWN_STR },
      pid{ UNKNOWN_UINT },
      row()
  {
  }
};

struct LogTableEntry
{
  std::string profile_name; // Refrences profile_name from ProfileTabEntry
  unsigned int pid;         // Partial key (with timestamp)
  time_t timestamp;         // Partial key (with pid)
  std::list<std::pair<std::string, std::string>> metadata;
  Gtk::TreeRow row;

  LogTableEntry(const time_t &timestamp,
                const std::string &profile_name,
                const unsigned int &pid,
                const std::list<std::pair<std::string, std::string>> &metadata,
                const Gtk::TreeRow &row)
    : profile_name{ profile_name },
      pid{ pid },
      timestamp{ timestamp },
      metadata{ metadata },
      row{ row }
  {
  }

  // Default constructor, used when no entry found
  LogTableEntry()
    : profile_name{ UNKNOWN_STR },
      pid{ UNKNOWN_UINT },
      timestamp{ UNKNOWN_TIME },
      row()
  {
  }
};

#endif // TABS_ENTRIES_H