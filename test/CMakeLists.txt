set(TEST_NAME test)
cmake_minimum_required (VERSION 3.16.3)

set(
  TEST_SOURCES
  ./src/test_runner.cc
  ./src/tabs/controller/logs_controller_test.cc
  ./src/tabs/controller/status_controller_test.cc
  ./src/tabs/model/log_adapter_test.cc
  ./src/tabs/model/process_adapter_test.cc
  ./src/tabs/model/profile_adapter_test.cc
  ./src/tabs/view/status_test.cc
  ./src/tabs/view/profiles_test.cc
  ./src/threads/command_caller.cc
  ./src/threads/dispatcher_middleman.cc
  ./src/threads/blocking_queue.cc
  ./aa-caller/aa-caller.cc
)

set(REPORT_FILES_DIR ${CMAKE_BINARY_DIR}/test/CMakeFiles/${TEST_NAME}.dir)

#====================================

find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)

#====================================

pkg_check_modules(GTEST gtest)
pkg_check_modules(GMOCK gmock)

if(${GTEST_FOUND} AND ${GMOCK_FOUND})
  message(STATUS "Adding unit tests to build")

  add_custom_command(
    OUTPUT ${RESOURCE_BUNDLE_OUTPUT}
    COMMAND glib-compile-resources --target=${RESOURCE_BUNDLE_OUTPUT} --generate-source ${XML_GRESOURCE}
    DEPENDS ${XML_GRESOURCE} ${GLADE_RESOURCES} 
    COMMENT "Generating Glib Resource Bundle."
    VERBATIM
  )

  add_executable(${TEST_NAME} EXCLUDE_FROM_ALL ${RESOURCE_BUNDLE_OUTPUT} ${SOURCES} ${TEST_SOURCES})

  # Do not use clang-tidy on test code
  set_target_properties(${TEST_NAME} PROPERTIES CXX_CLANG_TIDY "")

  # Set the flags for the "test" target
  set_target_properties(${TEST_NAME} PROPERTIES COMPILE_FLAGS " -fprofile-arcs -ftest-coverage -Wall -Wextra -g")
  set_target_properties(${TEST_NAME} PROPERTIES LINK_FLAGS    " -fprofile-arcs -ftest-coverage")

  # Define the "TESTS_ENABLED" preprocessor flag for the "test" target
  target_compile_definitions(${TEST_NAME} PUBLIC -DTESTS_ENABLED)

  target_include_directories(${TEST_NAME} SYSTEM PUBLIC ${GTKMM_INCLUDE_DIRS})

  target_link_libraries(${TEST_NAME} PUBLIC gtest)
  target_link_libraries(${TEST_NAME} PUBLIC gmock)
  target_link_libraries(${TEST_NAME} PUBLIC pthread)
  target_link_libraries(${TEST_NAME} PUBLIC jsoncpp)
  target_link_libraries(${TEST_NAME} PUBLIC ${GTKMM_LIBRARIES})
  target_link_libraries(${TEST_NAME} PUBLIC "aa-caller_dev")
  target_link_libraries(${TEST_NAME} PUBLIC apparmor-parser-dev)

  # Create target "report", which should run the unit-tests and generate a code coverage report  
  add_custom_target(
    LCOV_BASE
    COMMAND lcov -c -i --no-external --exclude */test/* --exclude *.c -d ${CMAKE_BINARY_DIR} -o ${REPORT_FILES_DIR}/base_coverage.info
    DEPENDS ${TEST_NAME}
  )

  add_custom_target(
    TEST_DATA
    COMMAND ${CMAKE_BINARY_DIR}/dist/${TEST_NAME}
    DEPENDS LCOV_BASE
  )

  add_custom_target(
    LCOV_TEST
    COMMAND lcov -c --no-external --exclude */test/* --exclude *.c -d ${CMAKE_BINARY_DIR} -o ${REPORT_FILES_DIR}/test_coverage.info
    DEPENDS TEST_DATA
  )

  add_custom_target(
    LCOV
    COMMAND lcov -a ${REPORT_FILES_DIR}/base_coverage.info -a ${REPORT_FILES_DIR}/test_coverage.info -o ${REPORT_FILES_DIR}/total_coverage.info
    DEPENDS LCOV_TEST
  )

  add_custom_target(
    report
    DEPENDS LCOV
    COMMAND genhtml ${REPORT_FILES_DIR}/total_coverage.info --output-directory ${PROJECT_SOURCE_DIR}/../report
  )

else()
  message(WARNING "Could not find googletest and googlemock packages. Please install them if you want to enable unit tests.")
endif()
