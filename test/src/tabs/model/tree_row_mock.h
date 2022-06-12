#ifndef TEST_SRC_TABS_MODEL_TREE_ROW_MOCK_TEST_H
#define TEST_SRC_TABS_MODEL_TREE_ROW_MOCK_TEST_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <gtkmm/treeiter.h>

class TreeRowMock : public Gtk::TreeRow
{
public:
  TreeRowMock() = default;
  TreeRowMock(const TreeRowMock&){};

  MOCK_METHOD(void, set_value, (int column, const std::string &data));
  MOCK_METHOD(void, set_value, (int column, const unsigned int& data));
};

#endif