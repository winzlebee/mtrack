# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_SOURCE_DIR = /home/win/Documents/Development/Personal/mtrack

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/win/Documents/Development/Personal/mtrack

# Include any dependencies generated for this target.
include CMakeFiles/mtrack.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/mtrack.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/mtrack.dir/flags.make

CMakeFiles/mtrack.dir/main.cpp.o: CMakeFiles/mtrack.dir/flags.make
CMakeFiles/mtrack.dir/main.cpp.o: main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/win/Documents/Development/Personal/mtrack/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/mtrack.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/mtrack.dir/main.cpp.o -c /home/win/Documents/Development/Personal/mtrack/main.cpp

CMakeFiles/mtrack.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mtrack.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/win/Documents/Development/Personal/mtrack/main.cpp > CMakeFiles/mtrack.dir/main.cpp.i

CMakeFiles/mtrack.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mtrack.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/win/Documents/Development/Personal/mtrack/main.cpp -o CMakeFiles/mtrack.dir/main.cpp.s

# Object files for target mtrack
mtrack_OBJECTS = \
"CMakeFiles/mtrack.dir/main.cpp.o"

# External object files for target mtrack
mtrack_EXTERNAL_OBJECTS =

mtrack: CMakeFiles/mtrack.dir/main.cpp.o
mtrack: CMakeFiles/mtrack.dir/build.make
mtrack: /usr/lib/x86_64-linux-gnu/libGL.so
mtrack: /usr/lib/x86_64-linux-gnu/libGLU.so
mtrack: CMakeFiles/mtrack.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/win/Documents/Development/Personal/mtrack/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable mtrack"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mtrack.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/mtrack.dir/build: mtrack

.PHONY : CMakeFiles/mtrack.dir/build

CMakeFiles/mtrack.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/mtrack.dir/cmake_clean.cmake
.PHONY : CMakeFiles/mtrack.dir/clean

CMakeFiles/mtrack.dir/depend:
	cd /home/win/Documents/Development/Personal/mtrack && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/win/Documents/Development/Personal/mtrack /home/win/Documents/Development/Personal/mtrack /home/win/Documents/Development/Personal/mtrack /home/win/Documents/Development/Personal/mtrack /home/win/Documents/Development/Personal/mtrack/CMakeFiles/mtrack.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/mtrack.dir/depend

