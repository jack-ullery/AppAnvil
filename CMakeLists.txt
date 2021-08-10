cmake_minimum_required (VERSION 3.16.3)

project (appanvil)

#### Set Source Code #####
set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/src)
set(
  SOURCES
  ${PROJECT_SOURCE_DIR}/main.cc
  ${PROJECT_SOURCE_DIR}/main_window.cc
  ${PROJECT_SOURCE_DIR}/tabs/hello_world.cc
)

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
    COMMAND cppcheck ${SOURCES}
    VERBATIM
  )
else()
  add_custom_target(
    CPPCHECK
    COMMAND echo "cppcheck: tool not found"
    VERBATIM
  )
endif()

find_program(CLANG_TIDY_EXISTS clang-tidy)
if(CLANG_TIDY_EXISTS)
  add_custom_target(
    CLANG-TIDY
    COMMAND echo "Running clang-tidy (this may take a while)..."
    COMMAND clang-tidy 
      -checks=-*,bugprone-*,cert-*,clang-analyzer-*,concurrency-*,cppcoreguidelines-*,google-*,hicpp-*,linuxkernel-*,llvm-*,llvmlibc-*,misc-*,performance-*,portability-*,readability-*
      -header-filter=.*,-gtkmm*
      ${SOURCES}
    VERBATIM
  )
else()
  add_custom_target(
    CLANG-TIDY
    COMMAND echo "clang-tidy: tool not found"
    VERBATIM
  )
endif()

#### Include GTKmm Libraries ####
find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)

link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(include ${GTKMM_INCLUDE_DIRS})

#### Set Compiler Options ####
set(CMAKE_CXX_FLAGS " -Wall -Wextra -g")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/dist)
set(CMAKE_LINK_WHAT_YOU_USE TRUE)

#### Link everything together ####
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${GTKMM_LIBRARIES})

