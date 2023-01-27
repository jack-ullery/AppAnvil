cmake_minimum_required (VERSION 3.16.3)

project (appanvil)

#### Set Source Code #####
set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/src)

### Sources that need to be built ###
set(
  SOURCES
  ${PROJECT_SOURCE_DIR}/main_window.cc
  ${PROJECT_SOURCE_DIR}/console_thread.cc
  ${PROJECT_SOURCE_DIR}/threads/blocking_queue.cc
  ${PROJECT_SOURCE_DIR}/threads/command_caller.cc
  ${PROJECT_SOURCE_DIR}/threads/dispatcher_middleman.cc
  ${PROJECT_SOURCE_DIR}/threads/log_record.cc
  ${PROJECT_SOURCE_DIR}/threads/log_reader.cc
  ${PROJECT_SOURCE_DIR}/tabs/model/database.cc
  ${PROJECT_SOURCE_DIR}/tabs/model/status_column_record.cc
  ${PROJECT_SOURCE_DIR}/tabs/model/edit_column_record.cc
  ${PROJECT_SOURCE_DIR}/tabs/model/profile_adapter.cc
  ${PROJECT_SOURCE_DIR}/tabs/model/process_adapter.cc
  ${PROJECT_SOURCE_DIR}/tabs/model/log_adapter.cc
  ${PROJECT_SOURCE_DIR}/tabs/controller/status_controller.cc
  ${PROJECT_SOURCE_DIR}/tabs/controller/profiles_controller.cc
  ${PROJECT_SOURCE_DIR}/tabs/controller/processes_controller.cc
  ${PROJECT_SOURCE_DIR}/tabs/controller/logs_controller.cc
  ${PROJECT_SOURCE_DIR}/tabs/view/status.cc
  ${PROJECT_SOURCE_DIR}/tabs/view/profiles.cc
  ${PROJECT_SOURCE_DIR}/tabs/view/processes.cc
  ${PROJECT_SOURCE_DIR}/tabs/view/logs.cc
  ${PROJECT_SOURCE_DIR}/tabs/view/profile_loader.cc
  ${PROJECT_SOURCE_DIR}/tabs/view/profile_modify.cc
  ${PROJECT_SOURCE_DIR}/tabs/view/help.cc
  ${PROJECT_SOURCE_DIR}/tabs/view/info_box.cc
  ${PROJECT_SOURCE_DIR}/tabs/view/edit_row.cc
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
  ${CMAKE_CURRENT_BINARY_DIR}/aa-caller/src/*.cc
  ${CMAKE_CURRENT_BINARY_DIR}/aa-caller/src/*.h
  ${CMAKE_CURRENT_BINARY_DIR}/aa-caller/src/*.inl
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
    set(CMAKE_CXX_CLANG_TIDY clang-tidy; --config-file ${CMAKE_CURRENT_BINARY_DIR}/.clang-tidy
    )
  endif()

  unset(USE_CLANG_TIDY CACHE)
endif()

# If clang-format is downloaded, format all the source code files in ./src and ./test/src
find_program(CLANG_FORMAT NAMES clang-format)
  if(${CLANG_FORMAT} STREQUAL "CLANG_FORMAT-NOTFOUND")
    message(WARNING "Could not find clang-format installation. Please install clang-format if you want to use it to automaticaly format code.")
  else()
    message(STATUS "Adding clang-format to build")
    add_custom_target(
      FORMAT
      COMMAND clang-format -style=file -i ${FORMAT_SOURCES}
    )
endif()

#### Include GTKmm Libraries ####
find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)

#### Include jsoncpp libraries ####
pkg_check_modules(JSONCPP jsoncpp)

#### Libapparmor used for parsing kernel logs ####
pkg_check_modules(LIBAPPARMOR libapparmor)

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
set(CMAKE_CXX_FLAGS " -Wall -Wextra")
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/dist)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(CMAKE_LINK_WHAT_YOU_USE TRUE)

#### Create the main executable ####
add_executable(${PROJECT_NAME} ${RESOURCE_BUNDLE_OUTPUT} ${SOURCES} ${PROJECT_SOURCE_DIR}/main.cc)

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${GTKMM_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${LIBAPPARMOR_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${jsoncpp_DIRS})

target_link_libraries(${PROJECT_NAME} PUBLIC ${GTKMM_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBAPPARMOR_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PUBLIC jsoncpp)
target_link_libraries(${PROJECT_NAME} PUBLIC pthread)
target_link_libraries(${PROJECT_NAME} PUBLIC apparmor-parser-dev)

pkg_check_modules(GTEST gtest)
pkg_check_modules(GMOCK gmock)

add_subdirectory(aa-caller)
add_subdirectory(test)

#### Create install target to install binaries  ####
install(
  TARGETS ${PROJECT_NAME}
  DESTINATION bin
)

set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libgtkmm-3.0-dev,libjsoncpp-dev,libapparmor-dev")
set(CPACK_PACKAGE_CONTACT "Jack Ullery")
set(CPACK_INSTALL_SCRIPT "cmake_install.cmake")
include(CPack)

#### Set install target to also modify pkexec ####
install(
  FILES ${PKEXEC_POLICY}
  DESTINATION /usr/share/polkit-1/actions/
)

#### Set install target to make snap profiles modifiable ####
# This creates symbolic links in `/var/lib/snapd/apparmor/profiles`, to make it easier to modify these profiles

set(PROFILE_SUBDIRS abi abstractions disable force-complain local tunables)
foreach(SUBDIR ${PROFILE_SUBDIRS})
  install(CODE "execute_process(
    COMMAND bash -c \"
      test -d /var/lib/snapd/apparmor/profiles &&
      ! (test -d /var/lib/snapd/apparmor/profiles/${SUBDIR}) &&
      sudo ln -s /etc/apparmor.d/${SUBDIR} /var/lib/snapd/apparmor/profiles
    \")")
endforeach( SUBDIR )

#### Creating uninstall target (copying code from CMake FAQ) ####
if(NOT TARGET uninstall)
  configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

  add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()
