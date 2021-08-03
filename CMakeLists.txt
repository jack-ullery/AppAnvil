cmake_minimum_required (VERSION 3.16.3)

project (appanvil)

find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)

link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(include ${GTKMM_INCLUDE_DIRS})

set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/src)

# Set compiler options
set(SOURCES
            ${PROJECT_SOURCE_DIR}/main.cc
            ${PROJECT_SOURCE_DIR}/main_window.cc        
            ${PROJECT_SOURCE_DIR}/tabs/hello_world.cc
    )

set(CMAKE_CXX_FLAGS " -Wall -Wextra -g")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/dist)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${GTKMM_LIBRARIES})

