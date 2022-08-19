set(TEST_NAME ${PROJECT_NAME}_test)
cmake_minimum_required (VERSION 3.16.3)

set(
  TEST_SOURCES
  ./src/test_runner.cc
  ./src/tabs/controller/logs_controller_test.cc
  ./src/tabs/controller/status_controller_test.cc
  ./src/tabs/model/database_mock.cc
  ./src/tabs/model/log_adapter_test.cc
  ./src/tabs/model/process_adapter_test.cc
  ./src/tabs/model/profile_adapter_test.cc
  ./src/tabs/view/status_test.cc
  ./src/threads/command_caller.cc
  ./src/threads/dispatcher_middleman.cc
  ./src/threads/blocking_queue.cc
)

set(REPORT_FILES_DIR ${CMAKE_BINARY_DIR}/test/CMakeFiles/${TEST_NAME}.dir)
message(${REPORT_FILES_DIR})

#====================================

find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)

#====================================

pkg_check_modules(GTEST gtest)
pkg_check_modules(GMOCK gmock)

if(${GTEST_FOUND} AND ${GMOCK_FOUND})
  message(STATUS "Adding unit tests to build")

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

  # Create target "REPORT", which should run the unit-tests and generate a code coverage report  
  add_custom_target(
    TEST_DATA
    COMMAND ${CMAKE_BINARY_DIR}/dist/appanvil_test
    DEPENDS appanvil_test
  )

  add_custom_target(
    LCOV
    COMMAND lcov -c --exclude '/usr/*' -d ${REPORT_FILES_DIR}/src --output-file ${REPORT_FILES_DIR}/test_coverage.info
    DEPENDS TEST_DATA
  )

  add_custom_target(
    REPORT
    DEPENDS LCOV
    COMMAND genhtml ${REPORT_FILES_DIR}/test_coverage.info --output-directory ${PROJECT_SOURCE_DIR}/../report
  )


else()
  message(WARNING "Could not find googletest and googlemock packages. Please install them if you want to enable unit tests.")
endif()
