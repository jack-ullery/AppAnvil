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

class TreeRowMock
{
public:
  MOCK_METHOD(void, set_value, (const uint &index, const std::string &data));
  MOCK_METHOD(void, set_value, (const uint &index, const unsigned long &data));
};

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
