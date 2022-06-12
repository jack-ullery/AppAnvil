#ifndef TEST_SRC_TABS_MODEL_DATABASE_MOCK_TEST_H
#define TEST_SRC_TABS_MODEL_DATABASE_MOCK_TEST_H

#include "../../../../src/tabs/entries.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

class DatabaseMock
{
public:
  DatabaseMock() = default;
  std::map<std::string, ProfileTableEntry> profile_data;
  std::map<std::string, std::map<uint, ProcessTableEntry>> process_data;
  std::map<std::string, std::map<time_t, LogTableEntry>> log_data;

  // Copied from the real Database
  uint get_number_processes(const std::string &profile);
};

#endif