# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

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
CMAKE_COMMAND = /home/smaket/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/192.7142.39/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/smaket/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/192.7142.39/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/smaket/CLionProjects/network/Sieci_produkcyjne

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/smaket/CLionProjects/network/Sieci_produkcyjne/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/network__debug.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/network__debug.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/network__debug.dir/flags.make

CMakeFiles/network__debug.dir/src/network.cpp.o: CMakeFiles/network__debug.dir/flags.make
CMakeFiles/network__debug.dir/src/network.cpp.o: ../src/network.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/smaket/CLionProjects/network/Sieci_produkcyjne/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/network__debug.dir/src/network.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/network__debug.dir/src/network.cpp.o -c /home/smaket/CLionProjects/network/Sieci_produkcyjne/src/network.cpp

CMakeFiles/network__debug.dir/src/network.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/network__debug.dir/src/network.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/smaket/CLionProjects/network/Sieci_produkcyjne/src/network.cpp > CMakeFiles/network__debug.dir/src/network.cpp.i

CMakeFiles/network__debug.dir/src/network.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/network__debug.dir/src/network.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/smaket/CLionProjects/network/Sieci_produkcyjne/src/network.cpp -o CMakeFiles/network__debug.dir/src/network.cpp.s

CMakeFiles/network__debug.dir/network_exec.cpp.o: CMakeFiles/network__debug.dir/flags.make
CMakeFiles/network__debug.dir/network_exec.cpp.o: ../network_exec.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/smaket/CLionProjects/network/Sieci_produkcyjne/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/network__debug.dir/network_exec.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/network__debug.dir/network_exec.cpp.o -c /home/smaket/CLionProjects/network/Sieci_produkcyjne/network_exec.cpp

CMakeFiles/network__debug.dir/network_exec.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/network__debug.dir/network_exec.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/smaket/CLionProjects/network/Sieci_produkcyjne/network_exec.cpp > CMakeFiles/network__debug.dir/network_exec.cpp.i

CMakeFiles/network__debug.dir/network_exec.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/network__debug.dir/network_exec.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/smaket/CLionProjects/network/Sieci_produkcyjne/network_exec.cpp -o CMakeFiles/network__debug.dir/network_exec.cpp.s

# Object files for target network__debug
network__debug_OBJECTS = \
"CMakeFiles/network__debug.dir/src/network.cpp.o" \
"CMakeFiles/network__debug.dir/network_exec.cpp.o"

# External object files for target network__debug
network__debug_EXTERNAL_OBJECTS =

network__debug: CMakeFiles/network__debug.dir/src/network.cpp.o
network__debug: CMakeFiles/network__debug.dir/network_exec.cpp.o
network__debug: CMakeFiles/network__debug.dir/build.make
network__debug: CMakeFiles/network__debug.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/smaket/CLionProjects/network/Sieci_produkcyjne/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable network__debug"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/network__debug.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/network__debug.dir/build: network__debug

.PHONY : CMakeFiles/network__debug.dir/build

CMakeFiles/network__debug.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/network__debug.dir/cmake_clean.cmake
.PHONY : CMakeFiles/network__debug.dir/clean

CMakeFiles/network__debug.dir/depend:
	cd /home/smaket/CLionProjects/network/Sieci_produkcyjne/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/smaket/CLionProjects/network/Sieci_produkcyjne /home/smaket/CLionProjects/network/Sieci_produkcyjne /home/smaket/CLionProjects/network/Sieci_produkcyjne/cmake-build-debug /home/smaket/CLionProjects/network/Sieci_produkcyjne/cmake-build-debug /home/smaket/CLionProjects/network/Sieci_produkcyjne/cmake-build-debug/CMakeFiles/network__debug.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/network__debug.dir/depend

