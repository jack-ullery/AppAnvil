#ifndef TABS_ENTRIES_H
#define TABS_ENTRIES_H

#include <gtkmm/treeiter.h>
#include <string>

constexpr auto UNKNOWN_STR  = "unknown";
constexpr uint UNKNOWN_UINT = -1;

struct TableEntry
{
  std::string profile_name; // The name of the profile for this entry

  explicit TableEntry(const std::string &profile_name) 
    : profile_name{ profile_name }
  { }
};

struct ProfileTableEntry : TableEntry
{
  std::string status;

  ProfileTableEntry(const std::string &_profile_name, const std::string &_status)
    : TableEntry(_profile_name),
      status{ _status }
  {
  }

  // Default constructor, used when no entry found
  ProfileTableEntry()
    : TableEntry(UNKNOWN_STR),
      status{ UNKNOWN_STR }      
  {
  }
};

struct ProcessTableEntry : TableEntry
{
  std::string process_name;
  const unsigned int pid;   // Unique, Primary Key

  ProcessTableEntry(const std::string &_profile_name, const std::string &_process_name, const unsigned int &_pid)
    : TableEntry(_profile_name),
      process_name{ _process_name },
      pid{ _pid }
  {
  }

  // Default constructor, used when no entry found
  ProcessTableEntry()
    : TableEntry(UNKNOWN_STR),
      pid{ UNKNOWN_UINT }
  {
  }
};

struct LogTableEntry : TableEntry
{
  const unsigned int pid;   // Partial key (with timestamp)
  const time_t timestamp;   // Partial key (with pid)
  std::string type;
  std::string operation;

  LogTableEntry(const time_t &_timestamp,
                const std::string &_type,
                const std::string &_operation,
                const std::string &_profile_name,
                const unsigned int &_pid)
    : TableEntry(_profile_name),
      pid{ _pid },
      timestamp{ _timestamp },
      type{ _type },
      operation{ _operation }
  {
  }

  // Default constructor, used when no entry found
  LogTableEntry()
    : TableEntry(UNKNOWN_STR),
      pid{ UNKNOWN_UINT },
      timestamp{ UNKNOWN_UINT },
      type{ UNKNOWN_STR },
      operation{ UNKNOWN_STR }
  {
  }
};

#endif // TABS_ENTRIES_H