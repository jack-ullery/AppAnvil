#ifndef TABS_MODEL_PROCESS_ADAPTER_H
#define TABS_MODEL_PROCESS_ADAPTER_H

#include <gtkmm/treeiter.h>
#include <map>
#include <memory>
#include <string>

#include "../column_header.h"
#include "../entries.h"
#include "status_column_record.h"

template<class Database> class ProcessAdapter
{
public:
  // Initializes the database adapter
  ProcessAdapter(std::shared_ptr<Database> db, const std::shared_ptr<Gtk::TreeView> &view, const std::shared_ptr<Gtk::ScrolledWindow> &win);

  void put_data(const std::string &profile_name, const unsigned int &pid,  const unsigned int &ppid, const std::string &user, const std::string &status);
  ProcessTableEntry get_data(const unsigned int &pid);

  std::shared_ptr<StatusColumnRecord> get_col_record();

protected:
  // Helper function for "put_data", creates and returns a new row.
  Gtk::TreeRow add_row(const std::string &profile_name, const unsigned int &pid,  const unsigned int &ppid, const std::string &user, const std::string &status);

private:
  std::shared_ptr<Database> db;

  const std::vector<ColumnHeader> col_names{ColumnHeader("Process Name"), 
                                            ColumnHeader("User"),
                                            ColumnHeader("Pid", ColumnHeader::ColumnType::INT), 
                                            ColumnHeader("Status")};

  std::shared_ptr<StatusColumnRecord> col_record;
};

#endif // TABS_MODEL_PROCESS_ADAPTER_H
