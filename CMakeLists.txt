cmake_minimum_required (VERSION 3.16.3)

project (appanvil)

#### Set Source Code #####
set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

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
  ${PROJECT_SOURCE_DIR}/tabs/model/combobox_store.cc
  ${PROJECT_SOURCE_DIR}/tabs/model/status_column_record.cc
  ${PROJECT_SOURCE_DIR}/tabs/model/profile_adapter.cc
  ${PROJECT_SOURCE_DIR}/tabs/model/process_adapter.cc
  ${PROJECT_SOURCE_DIR}/tabs/model/log_adapter.cc
  ${PROJECT_SOURCE_DIR}/tabs/controller/status_controller.cc
  ${PROJECT_SOURCE_DIR}/tabs/controller/profiles_controller.cc
  ${PROJECT_SOURCE_DIR}/tabs/controller/profile_modify_controller.cc
  ${PROJECT_SOURCE_DIR}/tabs/controller/profile_loader_controller.cc
  ${PROJECT_SOURCE_DIR}/tabs/controller/processes_controller.cc
  ${PROJECT_SOURCE_DIR}/tabs/controller/logs_controller.cc
  ${PROJECT_SOURCE_DIR}/tabs/view/apparmor_not_installed.cc
  ${PROJECT_SOURCE_DIR}/tabs/view/status.cc
  ${PROJECT_SOURCE_DIR}/tabs/view/profiles.cc
  ${PROJECT_SOURCE_DIR}/tabs/view/processes.cc
  ${PROJECT_SOURCE_DIR}/tabs/view/logs.cc
  ${PROJECT_SOURCE_DIR}/tabs/view/profile_loader.cc
  ${PROJECT_SOURCE_DIR}/tabs/view/profile_modify.cc
  ${PROJECT_SOURCE_DIR}/tabs/view/help.cc
  ${PROJECT_SOURCE_DIR}/tabs/view/info_box.cc
  ${PROJECT_SOURCE_DIR}/tabs/view/scrolled_view.cc
  ${PROJECT_SOURCE_DIR}/tabs/view/add_abstraction.cc
  ${PROJECT_SOURCE_DIR}/tabs/view/add_file_rule.cc
)

### All the code globbed together for easy formatting ###
# We include this so it can be affected by clang-format
file(GLOB_RECURSE FORMAT_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.inl
  ${CMAKE_CURRENT_SOURCE_DIR}/test/src/*.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/test/src/*.h
  ${CMAKE_CURRENT_SOURCE_DIR}/test/src/*.inl
  ${CMAKE_CURRENT_SOURCE_DIR}/aa-caller/src/*.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/aa-caller/src/*.h
  ${CMAKE_CURRENT_SOURCE_DIR}/aa-caller/src/*.inl
)

set(PROJECT_RESOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/resources)
set(PROJECT_UI_DIR ${PROJECT_RESOURCE_DIR}/ui)
set(GENERATED_DIR ${PROJECT_RESOURCE_DIR}/generated)

set(XML_GRESOURCE ${PROJECT_UI_DIR}/resources.gresource.xml)
set(RESOURCE_BUNDLE_OUTPUT ${GENERATED_DIR}/resource.autogen.c)

set(PKEXEC_POLICY_IN ${PROJECT_RESOURCE_DIR}/pkexec.policy.in)
set(PKEXEC_POLICY ${GENERATED_DIR}/com.github.jack-ullery.AppAnvil.pkexec.policy)
configure_file("${PKEXEC_POLICY_IN}" "${PKEXEC_POLICY}" @ONLY) 

set(LICENSE_FILE "${CMAKE_CURRENT_SOURCE_DIR}/docs/copyright")
set(ICON_FILE "${PROJECT_UI_DIR}/misc/icon.svg")

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
    set(CMAKE_CXX_CLANG_TIDY clang-tidy; --config-file ${CMAKE_CURRENT_SOURCE_DIR}/.clang-tidy)
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
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)

#### Include jsoncpp libraries ####
pkg_check_modules(JSONCPP REQUIRED jsoncpp)

#### libapparmor used for parsing kernel logs ####
pkg_check_modules(LIBAPPARMOR REQUIRED libapparmor)

#### libappanvil custom library for parsing/editing profiles ####
add_subdirectory(libappanvil)
set(LIBAPPANVIL_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/libappanvil")

#### Package the .gresource.xml resource bundle ####
# This gets the list of dependencies for ${XML_GRESOURCE} when cmake is called.
# If a dependency is added to the ${XML_GRESOURCE} file, you should run cmake again
execute_process(
  COMMAND glib-compile-resources ${XML_GRESOURCE} --generate-dependencies
  WORKING_DIRECTORY ${PROJECT_UI_DIR}
  OUTPUT_VARIABLE GLADE_RESOURCES
  RESULT_VARIABLE GLIB_RETURN_VALUE
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

message(STATUS "Using install prefix: ${CMAKE_INSTALL_PREFIX}")
set(APPANVIL_DESKTOP_IN ${PROJECT_RESOURCE_DIR}/appanvil.desktop.in)
set(APPANVIL_DESKTOP    ${GENERATED_DIR}/appanvil.desktop)
configure_file(${APPANVIL_DESKTOP_IN} ${APPANVIL_DESKTOP} @ONLY)

# This ensures that ${GLADE_RESOURCES} uses semicolons instead of newline characters as seperators
string(REGEX REPLACE "\n" ";" GLADE_RESOURCES "${GLADE_RESOURCES}")
list(TRANSFORM GLADE_RESOURCES PREPEND "${PROJECT_UI_DIR}/")

# If glib-compile-resources did not complete succesfully, throw a fatal error
if (NOT GLIB_RETURN_VALUE EQUAL 0)
    message(FATAL_ERROR "Failed to get the dependencies from glib-compile-resources")
endif()

add_custom_command(
  OUTPUT ${RESOURCE_BUNDLE_OUTPUT}
  COMMAND glib-compile-resources
            --target=${RESOURCE_BUNDLE_OUTPUT}
            --generate-source
            ${XML_GRESOURCE}
  WORKING_DIRECTORY ${PROJECT_UI_DIR}
  DEPENDS ${XML_GRESOURCE} ${GLADE_RESOURCES}
  COMMENT "Generating Glib Resource Bundle."
  VERBATIM
)

include_directories(
  SYSTEM # Adds -isystem instead of -I and this tells clang-tidy not to analyze these includes
  ${GENERATED_DIR}
)

#### Set Compiler Options ####
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -fPIE")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/dist)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(CMAKE_LINK_WHAT_YOU_USE TRUE)

#### Create the main executable ####
add_executable(${PROJECT_NAME} ${RESOURCE_BUNDLE_OUTPUT} ${SOURCES} ${PROJECT_SOURCE_DIR}/main.cc)
add_dependencies(${PROJECT_NAME} appanvil-dev)

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${GTKMM_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${LIBAPPARMOR_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${JSONCPP_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${LIBAPPANVIL_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} PUBLIC ${GTKMM_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBAPPARMOR_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PUBLIC ${JSONCPP_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PUBLIC appanvil-dev)
target_link_libraries(${PROJECT_NAME} PUBLIC pthread)

pkg_check_modules(GTEST gtest)
pkg_check_modules(GMOCK gmock)

enable_testing()
add_subdirectory(aa-caller)
add_subdirectory(test)

#### Create install target to install binaries  ####
install(
  TARGETS ${PROJECT_NAME}
  DESTINATION bin
)

#### Install desktop entry file ####
install(
  FILES ${APPANVIL_DESKTOP}
  DESTINATION share/applications
)

install(
  FILES ${LICENSE_FILE}
  DESTINATION "share/doc/appanvil"
  RENAME "copyright"
)

#### Install icon ####
install(
  FILES ${ICON_FILE}
  DESTINATION "share/icons"
  RENAME "${PROJECT_NAME}.svg"
)

#### Add CPack target ####
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
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

  add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake)
endif()
