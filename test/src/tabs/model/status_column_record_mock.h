#ifndef TEST_SRC_TABS_MODEL_STATUS_COLUMN_RECORD_MOCK_TEST_H
#define TEST_SRC_TABS_MODEL_STATUS_COLUMN_RECORD_MOCK_TEST_H

#include "tree_row_mock.h"
#include "../../../../src/tabs/column_header.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/treemodel.h>
#include <gtkmm/treemodelcolumn.h>
#include <gtkmm/treemodelfilter.h>
#include <gtkmm/treestore.h>
#include <gtkmm/treeview.h>
#include <tuple>

class StatusColumnRecordMock
{
public:
  static std::shared_ptr<StatusColumnRecordMock> create(const std::shared_ptr<Gtk::TreeView> &view,
                                                        const std::shared_ptr<Gtk::ScrolledWindow> &win,
                                                        const std::vector<ColumnHeader> &names)
  {
    std::ignore = view;
    std::ignore = win;
    std::ignore = names;
    return nullptr;
  }

  MOCK_METHOD(void, set_visible_func, (const Gtk::TreeModelFilter::SlotVisible &filter));
  MOCK_METHOD(TreeRowMock *, new_row, ());
  MOCK_METHOD(void, clear, ());
  MOCK_METHOD(uint, filter_rows, ());
  MOCK_METHOD(void, reselect_rows, ());
};

#endif