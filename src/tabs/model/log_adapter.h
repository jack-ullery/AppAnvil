#ifndef TABS_MODEL_LOG_ADAPTER_H
#define TABS_MODEL_LOG_ADAPTER_H

#include <gtkmm/scrolledwindow.h>
#include <gtkmm/treeview.h>
#include <map>
#include <memory>
#include <string>

#include "../column_header.h"
#include "../entries.h"

template<class Database, class ColumnRecord>
class LogAdapter
{
public:
  // Initializes the database adapter
  LogAdapter(std::shared_ptr<Database> db, const std::shared_ptr<Gtk::TreeView> &view, const std::shared_ptr<Gtk::ScrolledWindow> &win);

  // For unit-tests
  LogAdapter(std::shared_ptr<Database> db, std::shared_ptr<ColumnRecord> mock);

  void put_data(const time_t &timestamp,
                const std::string &type, 
                const std::string &operation,
                const std::string &profile_name,
                const unsigned int &pid, 
                const std::string &status);

  std::pair<LogTableEntry, bool> get_data(const std::string &profile_name, const time_t &timestamp);
  std::shared_ptr<ColumnRecord> get_col_record();

protected:
  static std::string format_log_data(const std::string &data);
  static std::string format_timestamp(const time_t &timestamp);

private:
  std::shared_ptr<Database> db;

  const std::vector<ColumnHeader> col_names{ColumnHeader("Time"),
                                            ColumnHeader("Type"),
                                            ColumnHeader("Operation"),
                                            ColumnHeader("Name"),
                                            ColumnHeader("Pid", ColumnHeader::ColumnType::INT),
                                            ColumnHeader("Status")};

  const std::shared_ptr<ColumnRecord> col_record;
};

#endif // TABS_MODEL_LOG_ADAPTER_H
