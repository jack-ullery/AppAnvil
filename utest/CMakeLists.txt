cmake_minimum_required (VERSION 3.16.3)

set(PROJECT_NAME testall)
set(
  UTEST_SOURCES
  ../src/utest/unittest.cc
  ../src/threads/command_caller.cc
  ../src/tabs/status.cc
)

project ( ${PROJECT_NAME} ) 

#====================================

find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)

#====================================

#add_executable(${PROJECT_NAME} ../src/utest/unittest.cc ../src/threads/command_caller.cc ../src/tabs/status.cc)
add_executable(${PROJECT_NAME} ${UTEST_SOURCES})

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${GTKMM_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} PUBLIC gtest)
target_link_libraries(${PROJECT_NAME} PUBLIC gmock)
target_link_libraries(${PROJECT_NAME} PUBLIC pthread)
target_link_libraries(${PROJECT_NAME} PUBLIC jsoncpp)
target_link_libraries(${PROJECT_NAME} PUBLIC ${GTKMM_LIBRARIES})