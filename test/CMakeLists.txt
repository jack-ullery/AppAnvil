set(TEST_NAME ${PROJECT_NAME}_test)
cmake_minimum_required (VERSION 3.16.3)

set(
  TEST_SOURCES
  ./src/test_runner.cc
  ./src/tabs/status.cc
  ./src/tabs/status_mock.cc
  ./src/tabs/profiles_mock.cc
  ./src/threads/command_caller.cc
  ./src/threads/dispatcher_middleman.cc
)

#====================================

find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)

#====================================

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../dist)
add_executable(${TEST_NAME} ${TEST_SOURCES})

# Do not use clang-tidy on test code
set_target_properties(${TEST_NAME} PROPERTIES CXX_CLANG_TIDY "")

target_include_directories(${TEST_NAME} SYSTEM PUBLIC ${GTKMM_INCLUDE_DIRS})

target_link_libraries(${TEST_NAME} PUBLIC gtest)
target_link_libraries(${TEST_NAME} PUBLIC gmock)
target_link_libraries(${TEST_NAME} PUBLIC pthread)
target_link_libraries(${TEST_NAME} PUBLIC jsoncpp)
target_link_libraries(${TEST_NAME} PUBLIC ${GTKMM_LIBRARIES})

target_link_libraries(${TEST_NAME} PUBLIC ${LIBRARY_OUTPUT_NAME})
