#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <gtkmm/treemodel.h>
#include <gtkmm/treemodelcolumn.h>
#include <gtkmm/treemodelfilter.h>
#include <gtkmm/treestore.h>
#include <gtkmm/treeview.h>

class TreeRowMock
{
public:
  MOCK_METHOD(void, set_value, (const uint &index, const std::string &data));
  MOCK_METHOD(void, set_value, (const uint &index, const unsigned long &data));
};

class StatusColumnRecordMock
{
public:
  MOCK_METHOD(std::shared_ptr<StatusColumnRecordMock>, create,
              (const std::shared_ptr<Gtk::TreeView> &view, std::vector<std::string> names));
  MOCK_METHOD(void, set_visible_func, (const Gtk::TreeModelFilter::SlotVisible &filter));
  MOCK_METHOD(TreeRowMock *, new_row, ());
  MOCK_METHOD(void, clear, ());
  MOCK_METHOD(uint, filter_rows, ());
};
