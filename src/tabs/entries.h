#ifndef TABS_ENTRIES_H
#define TABS_ENTRIES_H

#include <gtkmm/treeiter.h>
#include <string>

constexpr auto UNKNOWN_STR  = "unknown";
constexpr uint UNKNOWN_UINT = -1;

struct ProfileTableEntry {
  const std::string profile_name; // Unique, Primary Key
  std::string status;
  const Gtk::TreeRow row;

  ProfileTableEntry(const std::string &_profile_name, const std::string &_status, const Gtk::TreeRow &_row) 
  : profile_name{_profile_name}, 
    status{_status}, 
    row{_row} 
  { }

  // Default constructor, used when no entry found
  ProfileTableEntry()
  : profile_name{UNKNOWN_STR},
    status{UNKNOWN_STR},
    row()
  { }
};

struct ProcessTableEntry {
  std::string process_name;
  std::string profile_name; // Refrences profile_name from ProfileTabEntry (if not null)
  const unsigned int pid;   // Unique, Primary Key
  const Gtk::TreeRow row;

  ProcessTableEntry(const std::string &_process_name, const std::string &_profile_name, const unsigned int &_pid, const Gtk::TreeRow &_row)
  : process_name{_process_name},
    profile_name{_profile_name}, 
    pid{_pid}, 
    row{_row} 
  { }

  // Default constructor, used when no entry found
  ProcessTableEntry()
  : profile_name{UNKNOWN_STR},
    pid{UNKNOWN_UINT},
    row()
  { }
};

struct LogTableEntry {
  std::string profile_name; // Refrences profile_name from ProfileTabEntry
  const unsigned int pid;         // Partial key (with timestamp)
  const time_t timestamp;         // Partial key (with pid)
  std::string type;
  std::string operation;
  const Gtk::TreeRow row;

  LogTableEntry(const time_t &_timestamp,
                const std::string &_type,
                const std::string &_operation,
                const std::string &_profile_name,
                const unsigned int &_pid,
                const Gtk::TreeRow &_row)
  : profile_name{_profile_name}, 
    pid{_pid}, 
    timestamp{_timestamp}, 
    type{_type}, 
    operation{_operation}, 
    row{_row}
  { }

  // Default constructor, used when no entry found
  LogTableEntry()
  : profile_name{UNKNOWN_STR},
    pid{UNKNOWN_UINT},
    timestamp{UNKNOWN_UINT},
    type{UNKNOWN_STR},
    operation{UNKNOWN_STR},
    row()
  { }
};

#endif // TABS_ENTRIES_H