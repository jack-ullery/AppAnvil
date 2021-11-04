cmake_minimum_required (VERSION 3.16.3)

project (appanvil)

#### Set Source Code #####
set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/src)
set(
  SOURCES
  ${PROJECT_SOURCE_DIR}/main.cc
  ${PROJECT_SOURCE_DIR}/main_window.cc
  ${PROJECT_SOURCE_DIR}/console_thread.cc
  ${PROJECT_SOURCE_DIR}/threads/blocking_queue.h
  ${PROJECT_SOURCE_DIR}/threads/command_caller.cc
  ${PROJECT_SOURCE_DIR}/threads/dispatcher_middleman.cc
  ${PROJECT_SOURCE_DIR}/tabs/status_column_record.cc
  ${PROJECT_SOURCE_DIR}/tabs/status.cc
  ${PROJECT_SOURCE_DIR}/tabs/profiles.cc
  ${PROJECT_SOURCE_DIR}/tabs/processes.cc
  ${PROJECT_SOURCE_DIR}/tabs/logs.cc
)

set(PROJECT_RESOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/resources)
set(XML_GRESOURCE ${PROJECT_RESOURCE_DIR}/resources.gresource.xml)
set(RESOURCE_BUNDLE_OUTPUT ${PROJECT_RESOURCE_DIR}/resource.autogen.c)

#### Add Linters and Static Analysis ####
add_custom_target(
  ANALYZE
  DEPENDS
    CPPCHECK
    CLANG-TIDY
    ${SOURCES}
  VERBATIM
)

find_program(CPPCHECK_EXISTS cppcheck)
if(CPPCHECK_EXISTS)
  add_custom_target(
    CPPCHECK
    COMMAND cppcheck --project=compile_commands.json --inline-suppr --enable=all
    COMMENT "Running cppcheck..."
    VERBATIM
  )
else()
  add_custom_target(
    CPPCHECK
    COMMENT "cppcheck: tool not found"
    VERBATIM
  )
endif()

find_program(CLANG_TIDY_EXISTS clang-tidy)
if(CLANG_TIDY_EXISTS)
  add_custom_target(
    CLANG-TIDY
    COMMAND clang-tidy;
      -checks=-*,bugprone-*,cert-*,clang-analyzer-*,concurrency-*,cppcoreguidelines-*,google-*,hicpp-*,linuxkernel-*,llvm-*,llvmlibc-*,misc-*,performance-*,portability-*,readability-*,-llvm-header-guard*,-misc-non-private-member-variables-in-classes*
      -format-style='file'
      -header-filter=.*
      -p .
      ${SOURCES}
    COMMENT "Running clang-tidy (this may take a while)..."
    VERBATIM
  )
else()
  add_custom_target(
    CLANG-TIDY
    COMMENT "clang-tidy: tool not found"
    VERBATIM
  )
endif()

#### Include GTKmm Libraries ####
find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)

#link_directories(${GTKMM_LIBRARY_DIRS})
#include_directories(
#  header_only_library_no_static_code_analysis 
#  SYSTEM # Adds -isystem instead of -I and this tells clang-tidy not to analyze these includes
#  INTERFACE
#    ${GTKMM_INCLUDE_DIRS}
#)

#### Include jsoncpp libraries ####
pkg_check_modules(JSONCPP jsoncpp)

#### Package the .gresource.xml resource bundle ####

# This gets the list of dependencies for ${XML_GRESOURCE} when cmake is called.
# If a dependency is added to the ${XML_GRESOURCE} file, you should run cmake again
execute_process(
  COMMAND glib-compile-resources ${XML_GRESOURCE} --generate-dependencies 
  OUTPUT_VARIABLE GLADE_RESOURCES
  RESULT_VARIABLE GLIB_RETURN_VALUE
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

# This ensures that ${GLADE_RESOURCES} uses semicolons instead of newline characters as seperators
string(REGEX REPLACE "\n" ";" GLADE_RESOURCES "${GLADE_RESOURCES}")

# If glib-compile-resources did not complete succesfully, throw a fatal error
if (NOT GLIB_RETURN_VALUE EQUAL 0)
    message(FATAL_ERROR "Failed to get the dependencies from glib-compile-resources")
endif()

add_custom_command(
    OUTPUT ${RESOURCE_BUNDLE_OUTPUT}
    COMMAND glib-compile-resources --target=${RESOURCE_BUNDLE_OUTPUT} --generate-source ${XML_GRESOURCE}
    DEPENDS ${XML_GRESOURCE} ${GLADE_RESOURCES} 
    COMMENT "Generating Glib Resource Bundle."
    VERBATIM
)

include_directories(
  SYSTEM # Adds -isystem instead of -I and this tells clang-tidy not to analyze these includes
  ${PROJECT_RESOURCE_DIR}
)

#### Set Compiler Options ####
set(CMAKE_CXX_FLAGS " -Wall -Wextra -g")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/dist)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(CMAKE_LINK_WHAT_YOU_USE TRUE)
#set(CMAKE_CXX_CLANG_TIDY
#  clang-tidy;
#  -checks=-*,bugprone-*,cert-*,clang-analyzer-*,concurrency-*,cppcoreguidelines-*,google-*,hicpp-*,linuxkernel-*,llvm-*,llvmlibc-*,misc-*,performance-*,portability-*,readability-*,-llvm-header-guard*,-misc-non-private-member-variables-in-classes*,
#  -format-style='file';
#  -header-filter=${CMAKE_CURRENT_SOURCE_DIR};
#  -p .
#)

#### Link everything together ####
add_executable(${PROJECT_NAME} ${RESOURCE_BUNDLE_OUTPUT} ${SOURCES})

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${GTKMM_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${jsoncpp_DIRS})

target_link_libraries(${PROJECT_NAME} PUBLIC ${GTKMM_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PUBLIC jsoncpp)
target_link_libraries(${PROJECT_NAME} PUBLIC pthread)
