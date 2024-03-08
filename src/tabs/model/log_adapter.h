#ifndef TABS_MODEL_LOG_ADAPTER_H
#define TABS_MODEL_LOG_ADAPTER_H

#include <gtkmm/scrolledwindow.h>
#include <gtkmm/treeview.h>
#include <locale>
#include <map>
#include <memory>
#include <string>

#include "../column_header.h"
#include "../entries.h"

#ifdef TESTS_ENABLED
#include <gtest/gtest.h>
#endif

template<class Database, class ColumnRecord>
class LogAdapter
{
public:
  // Initializes the database adapter
  LogAdapter(std::shared_ptr<Database> db, const std::shared_ptr<Gtk::TreeView> &view);

  // For unit-tests
  LogAdapter(std::shared_ptr<Database> db, std::shared_ptr<ColumnRecord> mock);

  void put_data(const time_t &timestamp,
                const std::string &type,
                const std::string &operation,
                const std::string &profile_name,
                const unsigned int &pid,
                const std::list<std::pair<std::string, std::string>> &metadata);

  std::shared_ptr<ColumnRecord> get_col_record();

private:
  std::shared_ptr<Database> db;

  const std::vector<ColumnHeader> col_names{ ColumnHeader("Metadata", ColumnHeader::ColumnType::LOG_ENTRY),
                                             ColumnHeader("Time", ColumnHeader::ColumnType::TIMESTAMP),
                                             ColumnHeader("Type"),
                                             ColumnHeader("Operation"),
                                             ColumnHeader("Profile"),
                                             ColumnHeader("Pid", ColumnHeader::ColumnType::INT) };

  const std::shared_ptr<ColumnRecord> col_record;
};

#endif // TABS_MODEL_LOG_ADAPTER_H
