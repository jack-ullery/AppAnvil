#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <gtkmm/treemodel.h>
#include <gtkmm/treemodelcolumn.h>
#include <gtkmm/treemodelfilter.h>
#include <gtkmm/treestore.h>
#include <gtkmm/treeview.h>

class StatusColumnRecordMock
{
public:
  MOCK_METHOD(std::shared_ptr<StatusColumnRecord>, create, (const std::shared_ptr<Gtk::TreeView> &view, std::vector<std::string> names));
  MOCK_METHOD(void, set_visible_func, (const Gtk::TreeModelFilter::SlotVisible &filter));
  MOCK_METHOD(Gtk::TreeRow, new_row, ());
  MOCK_METHOD(void, set_row_data, (const Gtk::TreeRow &row, const uint &index, const std::string &data));
  MOCK_METHOD(void, clear, ());
  MOCK_METHOD(uint, filter_rows, ());
};
