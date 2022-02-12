project(aa-caller)
set(CALLER_NAME "aa-caller")
cmake_minimum_required (VERSION 3.16.3)

set(
  CALLER_SOURCES
  ./src/aa-caller.cc
)

#====================================

find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)

#====================================

message(STATUS "Adding aa-caller to build")

add_executable(${CALLER_NAME} ${CALLER_SOURCES})

# Do not use clang-tidy on test code
set_target_properties(${CALLER_NAME} PROPERTIES CXX_CLANG_TIDY "")

target_include_directories(${CALLER_NAME} SYSTEM PUBLIC ${GTKMM_INCLUDE_DIRS})

target_link_libraries(${CALLER_NAME} PUBLIC ${GTKMM_LIBRARIES})
target_link_libraries(${CALLER_NAME} PUBLIC pthread)
