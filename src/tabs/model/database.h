#ifndef TABS_DATABASE_H
#define TABS_DATABASE_H

#include <gtkmm/treeiter.h>
#include <map>
#include <memory>
#include <string>

#include "../column_header.h"
#include "status_column_record.h"

struct ProfileTableEntry {
  const std::string profile_name; // Unique, Primary Key
  std::string status;
  const Gtk::TreeRow row;

  ProfileTableEntry(const std::string _profile_name, const std::string _status, const Gtk::TreeRow _row) 
  : profile_name{_profile_name}, 
    status{_status}, 
    row{_row} 
  { }
};

struct ProcessTableEntry {
  const std::string profile_name; // Refrences profile_name from ProfileTabEntry
  const unsigned int pid;         // Unique
  const Gtk::TreeRow row;

  ProcessTableEntry(const std::string _profile_name, const unsigned int _pid, const Gtk::TreeRow _row)
  : profile_name{_profile_name}, 
    pid{_pid}, 
    row{_row} 
  { }
};

struct LogTableEntry {
  const std::string profile_name; // Refrences profile_name from ProfileTabEntry
  const unsigned int pid;
  const std::string timestamp;    // Unique (We assume)
  const std::string type;
  const std::string operation;
  const Gtk::TreeRow row;

  LogTableEntry(const std::string _profile_name, const unsigned int _pid, const std::string _timestamp,
                const std::string _type, const std::string _operation, const Gtk::TreeRow _row)
  : profile_name{_profile_name}, 
    pid{_pid}, 
    timestamp{_timestamp}, 
    type{_type}, 
    operation{_operation}, 
    row{_row}
  { }
};

class Database
{
public:
  // Initializes the database
  Database();

  void put_profile_data(const std::string profile_name, const std::string status);
  void put_process_data(const std::string profile_name, const unsigned int pid);
  void put_log_data(const std::string profile_name, const unsigned int pid, const std::string timestamp, const std::string type, const std::string operation);

  void register_profile_column_record(std::shared_ptr<StatusColumnRecord> col_record);

private:
  std::map<std::string, ProfileTableEntry> profile_data;
  std::map<std::string, ProcessTableEntry> process_data;
  std::map<std::string, LogTableEntry> log_data;

  std::shared_ptr<StatusColumnRecord> prof_col_record = NULL;
};

#endif // TABS_DATABASE_H
