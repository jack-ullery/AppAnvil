cmake_minimum_required (VERSION 3.16.3)

project (appanvil)

#### Set Source Code #####
set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/src)
### Sources that need to be built ###
set(
  SOURCES
  ${PROJECT_SOURCE_DIR}/main.cc
  ${PROJECT_SOURCE_DIR}/main_window.cc
  ${PROJECT_SOURCE_DIR}/console_thread.cc
  ${PROJECT_SOURCE_DIR}/threads/blocking_queue.h
  ${PROJECT_SOURCE_DIR}/threads/command_caller.cc
  ${PROJECT_SOURCE_DIR}/threads/dispatcher_middleman.h
  ${PROJECT_SOURCE_DIR}/tabs/status_column_record.cc
  ${PROJECT_SOURCE_DIR}/tabs/status.cc
  ${PROJECT_SOURCE_DIR}/tabs/profiles.cc
  ${PROJECT_SOURCE_DIR}/tabs/processes.cc
  ${PROJECT_SOURCE_DIR}/tabs/logs.cc
)

### All the code globbed together for easy formatting ###
# We include this so it can be affected by clang-format
file(GLOB_RECURSE FORMAT_SOURCES
  ${CMAKE_CURRENT_BINARY_DIR}/src/*.cc
  ${CMAKE_CURRENT_BINARY_DIR}/src/*.h
  ${CMAKE_CURRENT_BINARY_DIR}/src/*.inl
  ${CMAKE_CURRENT_BINARY_DIR}/test/src/*.cc
  ${CMAKE_CURRENT_BINARY_DIR}/test/src/*.h
  ${CMAKE_CURRENT_BINARY_DIR}/test/src/*.inl
)

set(PROJECT_RESOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/resources)
set(XML_GRESOURCE ${PROJECT_RESOURCE_DIR}/resources.gresource.xml)
set(RESOURCE_BUNDLE_OUTPUT ${PROJECT_RESOURCE_DIR}/resource.autogen.c)

set(PKEXEC_POLICY ${PROJECT_RESOURCE_DIR}/com.github.jack-ullery.AppAnvil.pkexec.policy)

#### Add Linters and Static Analysis ####

# If cmake was compiled with DANALYZE=TRUE
if(${ANALYZE})
  message(STATUS "Adding static-analysis tools to build:")
  set(USE_CLANG_TIDY TRUE)
  set(USE_CPPCHECK TRUE)
  unset(ANALYZE CACHE)
endif()

# If we want to use cppcheck
find_program(CPPCHECK NAMES cppcheck)

if(${USE_CPPCHECK})
  if(${CPPCHECK} STREQUAL "CPPCHECK-NOTFOUND")
    message(WARNING "Could not find cppcheck installation. Please install cppcheck if you want to use it for static analysis.")
  else()
    message(STATUS "Adding cppcheck to build")
    set(CMAKE_CXX_CPPCHECK cppcheck
      --enable=warning,performance,portability,information 
      --suppress=missingInclude --suppress=unmatchedSuppression --suppress=internalAstError 
      --quiet
      --inline-suppr
      src
    )
  endif()

  unset(USE_CPPCHECK CACHE)
endif()

# If we want to use clang-tidy
find_program(CLANG_TIDY NAMES clang-tidy)

if(${USE_CLANG_TIDY})
  if(${CLANG_TIDY} STREQUAL "CLANG_TIDY-NOTFOUND")
    message(WARNING "Could not find clang-tidy installation. Please install clang-tidy if you want to use it for static analysis.")
  else()
    message(STATUS "Adding clang-tidy to build")
    set(CMAKE_CXX_CLANG_TIDY clang-tidy;
      -checks=-*,bugprone-*,cert-*,clang-analyzer-*,concurrency-*,cppcoreguidelines-*,google-*,hicpp-*,linuxkernel-*,llvm-*,llvmlibc-*,misc-*,performance-*,portability-*,readability-*,-llvm-header-guard*,-misc-non-private-member-variables-in-classes*;
      -header-filter=./src/.*
    )
  endif()

  unset(USE_CLANG_TIDY CACHE)
endif()

# If clang-format is downloaded, format all the source code files in ./src and ./test/src
find_program(CLANG_FORMAT NAMES clang-format-12)
  if(${CLANG_FORMAT} STREQUAL "CLANG_FORMAT-NOTFOUND")
    message(WARNING "Could not find clang-format installation. Please install clang-format-12 if you want to use it to automaticaly format code.")
  else()
    message(STATUS "Adding clang-format-12 to build")
    add_custom_target(
      FORMAT
      COMMAND clang-format-12 -style=file -i ${FORMAT_SOURCES}
    )
endif()

#### Include GTKmm Libraries ####
find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)

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
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(CMAKE_LINK_WHAT_YOU_USE TRUE)

#### Create a library used by the binary and unit tests ####
set(LIBRARY_OUTPUT_NAME ${PROJECT_NAME}_main)
add_library(${LIBRARY_OUTPUT_NAME} STATIC ${RESOURCE_BUNDLE_OUTPUT} ${SOURCES})

target_include_directories(${LIBRARY_OUTPUT_NAME} SYSTEM PUBLIC ${GTKMM_INCLUDE_DIRS})
target_include_directories(${LIBRARY_OUTPUT_NAME} SYSTEM PUBLIC ${jsoncpp_DIRS})

target_link_libraries(${LIBRARY_OUTPUT_NAME} PUBLIC ${GTKMM_LIBRARIES})
target_link_libraries(${LIBRARY_OUTPUT_NAME} PUBLIC jsoncpp)
target_link_libraries(${LIBRARY_OUTPUT_NAME} PUBLIC pthread)

add_subdirectory(aa-caller)
add_subdirectory(test)

#### Create the binary to the library ####
add_executable(${PROJECT_NAME} ./src/main.cc)
target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBRARY_OUTPUT_NAME})

#### Create install target to install binaries  ####
install(
  TARGETS ${PROJECT_NAME}
  DESTINATION bin
)

#### Create install target to modify pkexec ####
install(
  FILES ${PKEXEC_POLICY}
  DESTINATION /usr/share/polkit-1/actions/
)

#### Creating uninstall target (copying code from CMake FAQ) ####
if(NOT TARGET uninstall)
  configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

  add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()
