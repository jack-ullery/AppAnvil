#ifndef TABS_DATABASE_H
#define TABS_DATABASE_H

#include <gtkmm/treeiter.h>
#include <map>
#include <string>

struct ProfileTableEntry {
  const std::string profile_name;
  const std::string status;
  const Gtk::TreeRow row;

  ProfileTableEntry(const std::string a, const std::string b, const Gtk::TreeRow c) : profile_name{a}, status{b}, row{c} { }
};

struct ProcessTableEntry {
  const std::string profile_name; // Refrences profile_name from ProfileTabEntry
  const unsigned int pid;
  const Gtk::TreeRow row;

  ProcessTableEntry(const std::string a, const unsigned int b, const Gtk::TreeRow c) : profile_name{a}, pid{b}, row{c} { }
};

struct LogTableEntry {
  const std::string profile_name; // Refrences profile_name from ProfileTabEntry
  const unsigned int pid;
  const std::string timestamp;
  const std::string type;
  const std::string operation;
  const Gtk::TreeRow row;

  LogTableEntry(const std::string a, const unsigned int b, const std::string c, const std::string d, const std::string e,
                const Gtk::TreeRow f)
      : profile_name{a}, pid{b}, timestamp{c}, type{d}, operation{e}, row{f}
  {
  }
};

class Database
{
public:
  // Initializes the database
  Database();

  void add_profile_data(const ProfileTableEntry &entry);
  void add_process_data(const ProcessTableEntry &entry);
  void add_log_data(const LogTableEntry &entry);

private:
  std::map<std::string, ProfileTableEntry> profile_data;
  std::map<std::string, ProcessTableEntry> process_data;
  std::map<std::string, LogTableEntry> log_data;
};

#endif // TABS_DATABASE_H
