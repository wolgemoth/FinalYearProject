# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /snap/clion/284/bin/cmake/linux/x64/bin/cmake

# The command to remove a file.
RM = /snap/clion/284/bin/cmake/linux/x64/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ubuntu/Projects/FinalYearProject/contrib/openal-soft/src/openal-soft

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ubuntu/Projects/FinalYearProject/contrib/openal-soft/src/openal-soft-build

# Include any dependencies generated for this target.
include CMakeFiles/alconvolve.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/alconvolve.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/alconvolve.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/alconvolve.dir/flags.make

CMakeFiles/alconvolve.dir/examples/alconvolve.c.o: CMakeFiles/alconvolve.dir/flags.make
CMakeFiles/alconvolve.dir/examples/alconvolve.c.o: /home/ubuntu/Projects/FinalYearProject/contrib/openal-soft/src/openal-soft/examples/alconvolve.c
CMakeFiles/alconvolve.dir/examples/alconvolve.c.o: CMakeFiles/alconvolve.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/ubuntu/Projects/FinalYearProject/contrib/openal-soft/src/openal-soft-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/alconvolve.dir/examples/alconvolve.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/alconvolve.dir/examples/alconvolve.c.o -MF CMakeFiles/alconvolve.dir/examples/alconvolve.c.o.d -o CMakeFiles/alconvolve.dir/examples/alconvolve.c.o -c /home/ubuntu/Projects/FinalYearProject/contrib/openal-soft/src/openal-soft/examples/alconvolve.c

CMakeFiles/alconvolve.dir/examples/alconvolve.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/alconvolve.dir/examples/alconvolve.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ubuntu/Projects/FinalYearProject/contrib/openal-soft/src/openal-soft/examples/alconvolve.c > CMakeFiles/alconvolve.dir/examples/alconvolve.c.i

CMakeFiles/alconvolve.dir/examples/alconvolve.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/alconvolve.dir/examples/alconvolve.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ubuntu/Projects/FinalYearProject/contrib/openal-soft/src/openal-soft/examples/alconvolve.c -o CMakeFiles/alconvolve.dir/examples/alconvolve.c.s

# Object files for target alconvolve
alconvolve_OBJECTS = \
"CMakeFiles/alconvolve.dir/examples/alconvolve.c.o"

# External object files for target alconvolve
alconvolve_EXTERNAL_OBJECTS =

alconvolve: CMakeFiles/alconvolve.dir/examples/alconvolve.c.o
alconvolve: CMakeFiles/alconvolve.dir/build.make
alconvolve: libcommon.a
alconvolve: /usr/lib/x86_64-linux-gnu/libsndfile.so
alconvolve: libex-common.a
alconvolve: libopenal.so.1.23.1
alconvolve: CMakeFiles/alconvolve.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/ubuntu/Projects/FinalYearProject/contrib/openal-soft/src/openal-soft-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable alconvolve"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/alconvolve.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/alconvolve.dir/build: alconvolve
.PHONY : CMakeFiles/alconvolve.dir/build

CMakeFiles/alconvolve.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/alconvolve.dir/cmake_clean.cmake
.PHONY : CMakeFiles/alconvolve.dir/clean

CMakeFiles/alconvolve.dir/depend:
	cd /home/ubuntu/Projects/FinalYearProject/contrib/openal-soft/src/openal-soft-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ubuntu/Projects/FinalYearProject/contrib/openal-soft/src/openal-soft /home/ubuntu/Projects/FinalYearProject/contrib/openal-soft/src/openal-soft /home/ubuntu/Projects/FinalYearProject/contrib/openal-soft/src/openal-soft-build /home/ubuntu/Projects/FinalYearProject/contrib/openal-soft/src/openal-soft-build /home/ubuntu/Projects/FinalYearProject/contrib/openal-soft/src/openal-soft-build/CMakeFiles/alconvolve.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/alconvolve.dir/depend

