cmake_minimum_required (VERSION 3.16.3)

project (appanvil)

find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)

link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(include ${GTKMM_INCLUDE_DIRS})

set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/src)

# # Figure out GLIB
# # Trying to call glib-compile-resources
# find_program(GLIB_COMPILE_RESOURCES NAMES glib-compile-resources REQUIRED)

# set(GRESOURCE_DIR   ${CMAKE_CURRENT_BINARY_DIR}/resources)
# set(GREESOURCE_GRE  ${GRESOURCE_DIR}/resources.gresource)
# set(GRESOURCE_XML   resources.xml)
# set(GRESOURCE_DEPS  main.glade hello.glade)

# # Step 2:
# add_custom_command(
#     OUTPUT ${GREESOURCE_GRE}
#     WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#     COMMAND ${GLIB_COMPILE_RESOURCES}
#     ARGS
#         --target=${GRESOURCE_GRE}
#         --generate-source ${GRESOURCE_DIR}/${GRESOURCE_XML}
#     VERBATIM
#     MAIN_DEPENDENCY ${GRESOURCE_DIR}/${GRESOURCE_XML}
#     DEPENDS ${GRESOURCE_DIR}/${GRESOURCE_DEPS}
# )

# # Step 3:
# add_custom_target(
#     glib-resource
#     DEPENDS ${GREESOURCE_GRE}
# )

# Set compiler options
set(SOURCES
            ${PROJECT_SOURCE_DIR}/main.cc
            ${PROJECT_SOURCE_DIR}/main_window.cc        
            ${PROJECT_SOURCE_DIR}/tabs/hello_world.cc
    )

set(CMAKE_CXX_FLAGS " -Wall -Wextra -g")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/dist)

add_executable(${PROJECT_NAME} ${SOURCES})

# Step 5:
# set_source_files_properties(${GREESOURCE_GRE} PROPERTIES GENERATED TRUE)
# THIS WILL BE NEEDED
# add_dependencies(${PROJECT_NAME} glib-resource) 

target_link_libraries(${PROJECT_NAME} ${GTKMM_LIBRARIES})

