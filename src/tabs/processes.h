#ifndef TABS_PROCESSES_H
#define TABS_PROCESSES_H

#include "status.h"

#include <memory>
#include <regex>
#include <string>

class Processes : public Status
{
public:
  Processes();
  virtual void add_data_to_record(const std::string &unconfined);
  void refresh();

protected:
  // Signal handlers
  void on_search_changed();

  bool filter_children(const Gtk::TreeModel::iterator &node) override;

  static void add_row_from_line(const std::shared_ptr<StatusColumnRecord> &col_record, const std::string &line);

private:
  const std::vector<ColumnHeader> col_names{ColumnHeader("Process Name"), ColumnHeader("User"),
                                            ColumnHeader("Pid", ColumnHeader::ColumnType::INT), ColumnHeader("Status")};
  std::shared_ptr<StatusColumnRecord> col_record;
};

#endif // TABS_PROCESSES_H
