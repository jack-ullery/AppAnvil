project(testall)
cmake_minimum_required (VERSION 3.16.3)

set(
  UTEST_SOURCES
  ./src/test_runner.cc
  ./src/status.cc
  ./src/threads/blocking_queue.cc
  ./src/threads/command_caller.cc
)

set(
  PROJECT_SOURCES
  ../src/threads/blocking_queue.h
  ../src/threads/command_caller.cc
  ../src/tabs/status.cc
)

#====================================

find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)

#====================================

#add_executable(${PROJECT_NAME} ../src/utest/unittest.cc ../src/threads/command_caller.cc ../src/tabs/status.cc)
add_executable(${PROJECT_NAME} ${UTEST_SOURCES} ${PROJECT_SOURCES})

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${GTKMM_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} PUBLIC gtest)
target_link_libraries(${PROJECT_NAME} PUBLIC gmock)
target_link_libraries(${PROJECT_NAME} PUBLIC pthread)
target_link_libraries(${PROJECT_NAME} PUBLIC jsoncpp)
target_link_libraries(${PROJECT_NAME} PUBLIC ${GTKMM_LIBRARIES})
