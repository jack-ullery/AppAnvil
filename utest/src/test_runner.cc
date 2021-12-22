//#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "status.test"
#include "threads/blocking_queue.test"
#include "threads/command_caller.test"

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}