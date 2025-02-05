cmake_minimum_required (VERSION 3.16.3)
project(appanvil_test)

set(
  APPANVIL_TEST_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/test_runner.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/tabs/controller/logs_controller_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/tabs/controller/status_controller_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/tabs/controller/status_controller_mock.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/tabs/model/log_adapter_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/tabs/model/process_adapter_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/tabs/model/profile_adapter_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/tabs/view/status_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/tabs/view/profiles_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/tabs/view/profile_modify_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/threads/command_caller.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/threads/dispatcher_middleman.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/threads/blocking_queue.cc
)

set(
  AA_CALLER_TEST_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/aa-caller/aa-caller.cc
)

set(REPORT_FILES_DIR ${CMAKE_BINARY_DIR}/${PROJECT_NAME}/CMakeFiles/${PROJECT_NAME}.dir)
set(EXAMPLE_PROFILE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/example_profiles)

#====================================

find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)

#====================================

pkg_check_modules(GTEST gtest)
pkg_check_modules(GMOCK gmock)

include(GoogleTest)

# Get source dirs
get_target_property(APPANVIL_SOURCE_DIR appanvil SOURCE_DIR)
set(APPANVIL_SOURCE_DIR "${APPANVIL_SOURCE_DIR}/src")

# Add the tests
if(${GTEST_FOUND} AND ${GMOCK_FOUND})
  message(STATUS "Adding unit tests to build")

  add_custom_command(
    OUTPUT ${RESOURCE_BUNDLE_OUTPUT}
    COMMAND glib-compile-resources
              --target=${RESOURCE_BUNDLE_OUTPUT}
              --sourcedir=${CMAKE_SOURCE_DIR}
              --generate-source
              ${XML_GRESOURCE}
    DEPENDS ${XML_GRESOURCE} ${GLADE_RESOURCES} 
    COMMENT "Generating Glib Resource Bundle."
    VERBATIM
  )

  #### Create test executable ####
  add_executable(
    ${PROJECT_NAME} EXCLUDE_FROM_ALL
      ${RESOURCE_BUNDLE_OUTPUT}
      ${SOURCES}
      ${APPANVIL_TEST_SOURCES}
      ${AA_CALLER_TEST_SOURCES}
  )

  # Do not use clang-tidy on test code
  set_target_properties(${PROJECT_NAME} PROPERTIES CXX_CLANG_TIDY "")

  # Set the flags for the "test" target
  target_compile_options(${PROJECT_NAME} PUBLIC 
#    "-fprofile-arcs"
#    "-ftest-coverage"
    "-Wall" 
    "-Wextra"
    "-g"
  )

  #target_link_options(${PROJECT_NAME} PUBLIC 
  #  "-fprofile-arcs"
  #  "-ftest-coverage"
  #)

  # Define the "TESTS_ENABLED" preprocessor flag for the "test" target
  target_compile_definitions(${PROJECT_NAME} PUBLIC -DTESTS_ENABLED)

  # Set the location of profiles as a global variable for the tests
  target_compile_definitions(${PROJECT_NAME} PUBLIC -DEXAMPLE_PROFILE_SOURCE_DIR="${EXAMPLE_PROFILE_SOURCE_DIR}")

  target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${GTKMM_INCLUDE_DIRS})
  target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${LIBAPPARMOR_INCLUDE_DIRS})
  target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${JSONCPP_INCLUDE_DIRS})
  target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${LIBAPPANVIL_INCLUDE_DIRS})
  target_include_directories(${PROJECT_NAME} PUBLIC ${APPANVIL_SOURCE_DIR})

  target_link_libraries(${PROJECT_NAME} PUBLIC gtest)
  target_link_libraries(${PROJECT_NAME} PUBLIC gmock)
  target_link_libraries(${PROJECT_NAME} PUBLIC gcov)
  target_link_libraries(${PROJECT_NAME} PUBLIC pthread)
  target_link_libraries(${PROJECT_NAME} PUBLIC ${JSONCPP_LIBRARIES})
  target_link_libraries(${PROJECT_NAME} PUBLIC ${GTKMM_LIBRARIES})
  target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBAPPARMOR_LIBRARIES})
  target_link_libraries(${PROJECT_NAME} PUBLIC "aa-caller_dev")
  target_link_libraries(${PROJECT_NAME} PUBLIC appanvil-dev)

  #### Add tests to ctest ####
  # To allow whitespace in test names
  cmake_policy(SET CMP0110 NEW)

  gtest_discover_tests(
    ${PROJECT_NAME}
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src"
    TEST_PREFIX "[appanvil]"
    TEST_LIST ADDED_TESTS_1
  )

  gtest_discover_tests(
    ${PROJECT_NAME}
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/aa-caller"
    TEST_PREFIX "[aa-caller] "
    TEST_LIST ADDED_TESTS_2
  )

  # #### Create fixture for tests ####
  # add_test(test_build
  #   "${CMAKE_COMMAND}"
  #   --build "${CMAKE_BINARY_DIR}"
  #   --config "$<CONFIG>"
  #   --target "${PROJECT_NAME}"
  # )

  # set_tests_properties(test_build PROPERTIES FIXTURES_SETUP test_fixture)
  # set_tests_properties(${ADDED_TESTS_1} ${ADDED_TESTS_2} PROPERTIES FIXTURES_REQUIRED test_fixture)

  #### Create report target ####
  # Create target "report", which should run the unit-tests and generate a code coverage report  
  add_custom_target(
    LCOV_BASE
    COMMAND lcov -c -i --no-external --exclude */${PROJECT_NAME}/* --exclude *.c -d ${CMAKE_SOURCE_DIR} -o ${REPORT_FILES_DIR}/base_coverage.info 1> /dev/null
    DEPENDS ${PROJECT_NAME}
    COMMENT "Create baseline coverage file"
  )

  add_custom_target(
    TEST_DATA
    COMMAND ${CMAKE_SOURCE_DIR}/dist/${PROJECT_NAME}
    DEPENDS LCOV_BASE
    COMMENT "Running unit tests"
  )

  add_custom_target(
    LCOV_TEST
    COMMAND lcov -c --no-external --exclude */${PROJECT_NAME}/* --exclude *.c -d ${CMAKE_SOURCE_DIR} -o ${REPORT_FILES_DIR}/test_coverage.info 1> /dev/null
    DEPENDS TEST_DATA
    COMMENT "Capture coverage after running tests"
  )

  add_custom_target(
    LCOV
    COMMAND lcov -a ${REPORT_FILES_DIR}/base_coverage.info -a ${REPORT_FILES_DIR}/test_coverage.info -o ${REPORT_FILES_DIR}/total_coverage.info
    DEPENDS LCOV_TEST
    COMMENT "Compare difference in coverage from baseline"
  )

  add_custom_target(
    report
    DEPENDS LCOV
    COMMAND genhtml ${REPORT_FILES_DIR}/total_coverage.info --output-directory ${PROJECT_SOURCE_DIR}/../report
    COMMENT "Generate HTML report"
  )

else()
  message(WARNING "Could not find googletest and googlemock packages. Please install them if you want to enable unit tests.")
endif()
