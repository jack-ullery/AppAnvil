# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/max/Desktop/AppAnvil-search-merged

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/max/Desktop/AppAnvil-search-merged

# Include any dependencies generated for this target.
include CMakeFiles/appanvil_main.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/appanvil_main.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/appanvil_main.dir/flags.make

resources/resource.autogen.c: resources/resources.gresource.xml
resources/resource.autogen.c: resources/status.glade
resources/resource.autogen.c: resources/about.glade
resources/resource.autogen.c: resources/icon.svg
resources/resource.autogen.c: resources/icon.glade
resources/resource.autogen.c: resources/load_profile.glade
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/max/Desktop/AppAnvil-search-merged/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating Glib Resource Bundle."
	glib-compile-resources --target=/home/max/Desktop/AppAnvil-search-merged/resources/resource.autogen.c --generate-source /home/max/Desktop/AppAnvil-search-merged/resources/resources.gresource.xml

CMakeFiles/appanvil_main.dir/resources/resource.autogen.c.o: CMakeFiles/appanvil_main.dir/flags.make
CMakeFiles/appanvil_main.dir/resources/resource.autogen.c.o: resources/resource.autogen.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/max/Desktop/AppAnvil-search-merged/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/appanvil_main.dir/resources/resource.autogen.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/appanvil_main.dir/resources/resource.autogen.c.o   -c /home/max/Desktop/AppAnvil-search-merged/resources/resource.autogen.c

CMakeFiles/appanvil_main.dir/resources/resource.autogen.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/appanvil_main.dir/resources/resource.autogen.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/max/Desktop/AppAnvil-search-merged/resources/resource.autogen.c > CMakeFiles/appanvil_main.dir/resources/resource.autogen.c.i

CMakeFiles/appanvil_main.dir/resources/resource.autogen.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/appanvil_main.dir/resources/resource.autogen.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/max/Desktop/AppAnvil-search-merged/resources/resource.autogen.c -o CMakeFiles/appanvil_main.dir/resources/resource.autogen.c.s

CMakeFiles/appanvil_main.dir/src/main.cc.o: CMakeFiles/appanvil_main.dir/flags.make
CMakeFiles/appanvil_main.dir/src/main.cc.o: src/main.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/max/Desktop/AppAnvil-search-merged/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/appanvil_main.dir/src/main.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/appanvil_main.dir/src/main.cc.o -c /home/max/Desktop/AppAnvil-search-merged/src/main.cc

CMakeFiles/appanvil_main.dir/src/main.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/appanvil_main.dir/src/main.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/max/Desktop/AppAnvil-search-merged/src/main.cc > CMakeFiles/appanvil_main.dir/src/main.cc.i

CMakeFiles/appanvil_main.dir/src/main.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/appanvil_main.dir/src/main.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/max/Desktop/AppAnvil-search-merged/src/main.cc -o CMakeFiles/appanvil_main.dir/src/main.cc.s

CMakeFiles/appanvil_main.dir/src/main_window.cc.o: CMakeFiles/appanvil_main.dir/flags.make
CMakeFiles/appanvil_main.dir/src/main_window.cc.o: src/main_window.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/max/Desktop/AppAnvil-search-merged/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/appanvil_main.dir/src/main_window.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/appanvil_main.dir/src/main_window.cc.o -c /home/max/Desktop/AppAnvil-search-merged/src/main_window.cc

CMakeFiles/appanvil_main.dir/src/main_window.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/appanvil_main.dir/src/main_window.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/max/Desktop/AppAnvil-search-merged/src/main_window.cc > CMakeFiles/appanvil_main.dir/src/main_window.cc.i

CMakeFiles/appanvil_main.dir/src/main_window.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/appanvil_main.dir/src/main_window.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/max/Desktop/AppAnvil-search-merged/src/main_window.cc -o CMakeFiles/appanvil_main.dir/src/main_window.cc.s

CMakeFiles/appanvil_main.dir/src/console_thread.cc.o: CMakeFiles/appanvil_main.dir/flags.make
CMakeFiles/appanvil_main.dir/src/console_thread.cc.o: src/console_thread.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/max/Desktop/AppAnvil-search-merged/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/appanvil_main.dir/src/console_thread.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/appanvil_main.dir/src/console_thread.cc.o -c /home/max/Desktop/AppAnvil-search-merged/src/console_thread.cc

CMakeFiles/appanvil_main.dir/src/console_thread.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/appanvil_main.dir/src/console_thread.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/max/Desktop/AppAnvil-search-merged/src/console_thread.cc > CMakeFiles/appanvil_main.dir/src/console_thread.cc.i

CMakeFiles/appanvil_main.dir/src/console_thread.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/appanvil_main.dir/src/console_thread.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/max/Desktop/AppAnvil-search-merged/src/console_thread.cc -o CMakeFiles/appanvil_main.dir/src/console_thread.cc.s

CMakeFiles/appanvil_main.dir/src/threads/command_caller.cc.o: CMakeFiles/appanvil_main.dir/flags.make
CMakeFiles/appanvil_main.dir/src/threads/command_caller.cc.o: src/threads/command_caller.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/max/Desktop/AppAnvil-search-merged/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/appanvil_main.dir/src/threads/command_caller.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/appanvil_main.dir/src/threads/command_caller.cc.o -c /home/max/Desktop/AppAnvil-search-merged/src/threads/command_caller.cc

CMakeFiles/appanvil_main.dir/src/threads/command_caller.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/appanvil_main.dir/src/threads/command_caller.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/max/Desktop/AppAnvil-search-merged/src/threads/command_caller.cc > CMakeFiles/appanvil_main.dir/src/threads/command_caller.cc.i

CMakeFiles/appanvil_main.dir/src/threads/command_caller.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/appanvil_main.dir/src/threads/command_caller.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/max/Desktop/AppAnvil-search-merged/src/threads/command_caller.cc -o CMakeFiles/appanvil_main.dir/src/threads/command_caller.cc.s

CMakeFiles/appanvil_main.dir/src/tabs/status_column_record.cc.o: CMakeFiles/appanvil_main.dir/flags.make
CMakeFiles/appanvil_main.dir/src/tabs/status_column_record.cc.o: src/tabs/status_column_record.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/max/Desktop/AppAnvil-search-merged/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/appanvil_main.dir/src/tabs/status_column_record.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/appanvil_main.dir/src/tabs/status_column_record.cc.o -c /home/max/Desktop/AppAnvil-search-merged/src/tabs/status_column_record.cc

CMakeFiles/appanvil_main.dir/src/tabs/status_column_record.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/appanvil_main.dir/src/tabs/status_column_record.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/max/Desktop/AppAnvil-search-merged/src/tabs/status_column_record.cc > CMakeFiles/appanvil_main.dir/src/tabs/status_column_record.cc.i

CMakeFiles/appanvil_main.dir/src/tabs/status_column_record.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/appanvil_main.dir/src/tabs/status_column_record.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/max/Desktop/AppAnvil-search-merged/src/tabs/status_column_record.cc -o CMakeFiles/appanvil_main.dir/src/tabs/status_column_record.cc.s

CMakeFiles/appanvil_main.dir/src/tabs/status.cc.o: CMakeFiles/appanvil_main.dir/flags.make
CMakeFiles/appanvil_main.dir/src/tabs/status.cc.o: src/tabs/status.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/max/Desktop/AppAnvil-search-merged/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/appanvil_main.dir/src/tabs/status.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/appanvil_main.dir/src/tabs/status.cc.o -c /home/max/Desktop/AppAnvil-search-merged/src/tabs/status.cc

CMakeFiles/appanvil_main.dir/src/tabs/status.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/appanvil_main.dir/src/tabs/status.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/max/Desktop/AppAnvil-search-merged/src/tabs/status.cc > CMakeFiles/appanvil_main.dir/src/tabs/status.cc.i

CMakeFiles/appanvil_main.dir/src/tabs/status.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/appanvil_main.dir/src/tabs/status.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/max/Desktop/AppAnvil-search-merged/src/tabs/status.cc -o CMakeFiles/appanvil_main.dir/src/tabs/status.cc.s

CMakeFiles/appanvil_main.dir/src/tabs/profiles.cc.o: CMakeFiles/appanvil_main.dir/flags.make
CMakeFiles/appanvil_main.dir/src/tabs/profiles.cc.o: src/tabs/profiles.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/max/Desktop/AppAnvil-search-merged/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/appanvil_main.dir/src/tabs/profiles.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/appanvil_main.dir/src/tabs/profiles.cc.o -c /home/max/Desktop/AppAnvil-search-merged/src/tabs/profiles.cc

CMakeFiles/appanvil_main.dir/src/tabs/profiles.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/appanvil_main.dir/src/tabs/profiles.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/max/Desktop/AppAnvil-search-merged/src/tabs/profiles.cc > CMakeFiles/appanvil_main.dir/src/tabs/profiles.cc.i

CMakeFiles/appanvil_main.dir/src/tabs/profiles.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/appanvil_main.dir/src/tabs/profiles.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/max/Desktop/AppAnvil-search-merged/src/tabs/profiles.cc -o CMakeFiles/appanvil_main.dir/src/tabs/profiles.cc.s

CMakeFiles/appanvil_main.dir/src/tabs/processes.cc.o: CMakeFiles/appanvil_main.dir/flags.make
CMakeFiles/appanvil_main.dir/src/tabs/processes.cc.o: src/tabs/processes.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/max/Desktop/AppAnvil-search-merged/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/appanvil_main.dir/src/tabs/processes.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/appanvil_main.dir/src/tabs/processes.cc.o -c /home/max/Desktop/AppAnvil-search-merged/src/tabs/processes.cc

CMakeFiles/appanvil_main.dir/src/tabs/processes.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/appanvil_main.dir/src/tabs/processes.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/max/Desktop/AppAnvil-search-merged/src/tabs/processes.cc > CMakeFiles/appanvil_main.dir/src/tabs/processes.cc.i

CMakeFiles/appanvil_main.dir/src/tabs/processes.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/appanvil_main.dir/src/tabs/processes.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/max/Desktop/AppAnvil-search-merged/src/tabs/processes.cc -o CMakeFiles/appanvil_main.dir/src/tabs/processes.cc.s

CMakeFiles/appanvil_main.dir/src/tabs/file_chooser.cc.o: CMakeFiles/appanvil_main.dir/flags.make
CMakeFiles/appanvil_main.dir/src/tabs/file_chooser.cc.o: src/tabs/file_chooser.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/max/Desktop/AppAnvil-search-merged/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/appanvil_main.dir/src/tabs/file_chooser.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/appanvil_main.dir/src/tabs/file_chooser.cc.o -c /home/max/Desktop/AppAnvil-search-merged/src/tabs/file_chooser.cc

CMakeFiles/appanvil_main.dir/src/tabs/file_chooser.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/appanvil_main.dir/src/tabs/file_chooser.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/max/Desktop/AppAnvil-search-merged/src/tabs/file_chooser.cc > CMakeFiles/appanvil_main.dir/src/tabs/file_chooser.cc.i

CMakeFiles/appanvil_main.dir/src/tabs/file_chooser.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/appanvil_main.dir/src/tabs/file_chooser.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/max/Desktop/AppAnvil-search-merged/src/tabs/file_chooser.cc -o CMakeFiles/appanvil_main.dir/src/tabs/file_chooser.cc.s

CMakeFiles/appanvil_main.dir/src/tabs/about.cc.o: CMakeFiles/appanvil_main.dir/flags.make
CMakeFiles/appanvil_main.dir/src/tabs/about.cc.o: src/tabs/about.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/max/Desktop/AppAnvil-search-merged/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/appanvil_main.dir/src/tabs/about.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/appanvil_main.dir/src/tabs/about.cc.o -c /home/max/Desktop/AppAnvil-search-merged/src/tabs/about.cc

CMakeFiles/appanvil_main.dir/src/tabs/about.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/appanvil_main.dir/src/tabs/about.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/max/Desktop/AppAnvil-search-merged/src/tabs/about.cc > CMakeFiles/appanvil_main.dir/src/tabs/about.cc.i

CMakeFiles/appanvil_main.dir/src/tabs/about.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/appanvil_main.dir/src/tabs/about.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/max/Desktop/AppAnvil-search-merged/src/tabs/about.cc -o CMakeFiles/appanvil_main.dir/src/tabs/about.cc.s

# Object files for target appanvil_main
appanvil_main_OBJECTS = \
"CMakeFiles/appanvil_main.dir/resources/resource.autogen.c.o" \
"CMakeFiles/appanvil_main.dir/src/main.cc.o" \
"CMakeFiles/appanvil_main.dir/src/main_window.cc.o" \
"CMakeFiles/appanvil_main.dir/src/console_thread.cc.o" \
"CMakeFiles/appanvil_main.dir/src/threads/command_caller.cc.o" \
"CMakeFiles/appanvil_main.dir/src/tabs/status_column_record.cc.o" \
"CMakeFiles/appanvil_main.dir/src/tabs/status.cc.o" \
"CMakeFiles/appanvil_main.dir/src/tabs/profiles.cc.o" \
"CMakeFiles/appanvil_main.dir/src/tabs/processes.cc.o" \
"CMakeFiles/appanvil_main.dir/src/tabs/file_chooser.cc.o" \
"CMakeFiles/appanvil_main.dir/src/tabs/about.cc.o"

# External object files for target appanvil_main
appanvil_main_EXTERNAL_OBJECTS =

lib/libappanvil_main.a: CMakeFiles/appanvil_main.dir/resources/resource.autogen.c.o
lib/libappanvil_main.a: CMakeFiles/appanvil_main.dir/src/main.cc.o
lib/libappanvil_main.a: CMakeFiles/appanvil_main.dir/src/main_window.cc.o
lib/libappanvil_main.a: CMakeFiles/appanvil_main.dir/src/console_thread.cc.o
lib/libappanvil_main.a: CMakeFiles/appanvil_main.dir/src/threads/command_caller.cc.o
lib/libappanvil_main.a: CMakeFiles/appanvil_main.dir/src/tabs/status_column_record.cc.o
lib/libappanvil_main.a: CMakeFiles/appanvil_main.dir/src/tabs/status.cc.o
lib/libappanvil_main.a: CMakeFiles/appanvil_main.dir/src/tabs/profiles.cc.o
lib/libappanvil_main.a: CMakeFiles/appanvil_main.dir/src/tabs/processes.cc.o
lib/libappanvil_main.a: CMakeFiles/appanvil_main.dir/src/tabs/file_chooser.cc.o
lib/libappanvil_main.a: CMakeFiles/appanvil_main.dir/src/tabs/about.cc.o
lib/libappanvil_main.a: CMakeFiles/appanvil_main.dir/build.make
lib/libappanvil_main.a: CMakeFiles/appanvil_main.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/max/Desktop/AppAnvil-search-merged/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Linking CXX static library lib/libappanvil_main.a"
	$(CMAKE_COMMAND) -P CMakeFiles/appanvil_main.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/appanvil_main.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/appanvil_main.dir/build: lib/libappanvil_main.a

.PHONY : CMakeFiles/appanvil_main.dir/build

CMakeFiles/appanvil_main.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/appanvil_main.dir/cmake_clean.cmake
.PHONY : CMakeFiles/appanvil_main.dir/clean

CMakeFiles/appanvil_main.dir/depend: resources/resource.autogen.c
	cd /home/max/Desktop/AppAnvil-search-merged && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/max/Desktop/AppAnvil-search-merged /home/max/Desktop/AppAnvil-search-merged /home/max/Desktop/AppAnvil-search-merged /home/max/Desktop/AppAnvil-search-merged /home/max/Desktop/AppAnvil-search-merged/CMakeFiles/appanvil_main.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/appanvil_main.dir/depend

