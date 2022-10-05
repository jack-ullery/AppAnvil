#ifndef TABS_MODEL_PROCESS_ADAPTER_H
#define TABS_MODEL_PROCESS_ADAPTER_H

#include <gtkmm/scrolledwindow.h>
#include <gtkmm/treeiter.h>
#include <gtkmm/treeview.h>
#include <map>
#include <memory>
#include <string>

#include "../column_header.h"
#include "../entries.h"

template<class Database, class ColumnRecord>
class ProcessAdapter
{
public:
  // Initializes the database adapter
  ProcessAdapter(std::shared_ptr<Database> db, const std::shared_ptr<Gtk::TreeView> &view, const std::shared_ptr<Gtk::ScrolledWindow> &win);

  // For unit-tests
  ProcessAdapter(std::shared_ptr<Database> db, std::shared_ptr<ColumnRecord> mock);

  void put_data(const std::string &process_name,
                const std::string &profile_name,
                const unsigned int &pid,
                const unsigned int &ppid,
                const std::string &user,
                const std::string &status);

  std::pair<ProcessTableEntry, bool> get_data(std::string profile_name, const unsigned int &pid);

  std::shared_ptr<ColumnRecord> get_col_record();

protected:
  // Helper function for "put_data", creates and returns a new row.
  ProcessTableEntry add_row(const std::string &profile_name,
                            const std::string &process_name,
                            const unsigned int &pid,
                            const unsigned int &ppid,
                            const std::string &user,
                            const std::string &status);

private:
  std::shared_ptr<Database> db;

  const std::vector<ColumnHeader> col_names{ ColumnHeader("Metadata", ColumnHeader::ColumnType::PROCESS_ENTRY),
                                             ColumnHeader("Process Name"),
                                             ColumnHeader("User"),
                                             ColumnHeader("Pid", ColumnHeader::ColumnType::INT),
                                             ColumnHeader("Status") };

  std::shared_ptr<ColumnRecord> col_record;
};

#endif // TABS_MODEL_PROCESS_ADAPTER_H
