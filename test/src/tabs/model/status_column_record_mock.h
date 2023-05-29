#ifndef TEST_SRC_TABS_MODEL_STATUS_COLUMN_RECORD_MOCK_TEST_H
#define TEST_SRC_TABS_MODEL_STATUS_COLUMN_RECORD_MOCK_TEST_H

#include "../../../../src/tabs/column_header.h"
#include "../../../../src/tabs/model/status_column_record.h"
#include "tree_row_mock.h"
// #include "../../../../src/tabs/view/status.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/treeiter.h>
#include <gtkmm/treemodel.h>
#include <gtkmm/treemodelcolumn.h>
#include <gtkmm/treemodelfilter.h>
#include <gtkmm/treestore.h>
#include <gtkmm/treeview.h>
#include <memory>
#include <tuple>

class StatusColumnRecordMock
{
public:
  static std::shared_ptr<StatusColumnRecordMock> create(const std::shared_ptr<Gtk::TreeView> &view,
                                                        const std::vector<ColumnHeader> &names)
  {
    std::ignore = view;
    std::ignore = names;
    return nullptr;
  }

  // StatusColumnRecordMock()
  //   : StatusColumnRecord(tab, col_names)
  // {  }

  MOCK_METHOD(void, set_visible_func, (const Gtk::TreeModelFilter::SlotVisible &filter));
  MOCK_METHOD(Gtk::TreeRow, new_row, ());
  MOCK_METHOD(Gtk::TreeRow, new_child_row, (const Gtk::TreeRow &parent));
  MOCK_METHOD(uint, filter_rows, ());
  MOCK_METHOD(Gtk::TreeRow, get_parent_by_pid, (unsigned int pid, const Gtk::TreeRow &parent));
  MOCK_METHOD(Gtk::TreeRow, get_parent_by_pid, (unsigned int pid));

private:
  const std::vector<ColumnHeader> col_names{ ColumnHeader("Time"),
                                             ColumnHeader("Type"),
                                             ColumnHeader("Operation"),
                                             ColumnHeader("Name"),
                                             ColumnHeader("Pid", ColumnHeader::ColumnType::INT),
                                             ColumnHeader("Status") };
  // const std::shared_ptr<Status> tab{new Status()};
};

#endif