#ifndef TEST_SRC_TABS_MODEL_TREE_ROW_MOCK_TEST_H
#define TEST_SRC_TABS_MODEL_TREE_ROW_MOCK_TEST_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>

class TreeRowMock
{
public:
  MOCK_METHOD(void, set_value, (const uint &index, const std::string &data));
  MOCK_METHOD(void, set_value, (const uint &index, const unsigned long &data));
};

#endif