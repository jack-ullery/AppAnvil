project(aa-caller)
set(CALLER_NAME "aa-caller")
cmake_minimum_required (VERSION 3.16.3)

set(
  CALLER_SOURCES
  ./src/aa-caller.cc
  ./src/main.cc
)

#====================================

find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)

#====================================

message(STATUS "Adding aa-caller to build")

set(CALLER_LIBRARY_NAME ${PROJECT_NAME}_dev)
add_library(${CALLER_LIBRARY_NAME} STATIC ${CALLER_SOURCES})

target_include_directories(${CALLER_LIBRARY_NAME} SYSTEM PUBLIC ${GTKMM_INCLUDE_DIRS})

target_link_libraries(${CALLER_LIBRARY_NAME} PUBLIC ${GTKMM_LIBRARIES})
target_link_libraries(${CALLER_LIBRARY_NAME} PUBLIC pthread)

add_executable(${CALLER_NAME} ./src/main.cc)
target_link_libraries(${CALLER_NAME} PUBLIC ${CALLER_LIBRARY_NAME})

#### Create install target to install binaries  ####
install(
  TARGETS ${CALLER_NAME}
  DESTINATION bin
)