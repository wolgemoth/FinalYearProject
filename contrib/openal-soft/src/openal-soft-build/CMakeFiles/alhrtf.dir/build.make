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
include CMakeFiles/alhrtf.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/alhrtf.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/alhrtf.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/alhrtf.dir/flags.make

CMakeFiles/alhrtf.dir/examples/alhrtf.c.o: CMakeFiles/alhrtf.dir/flags.make
CMakeFiles/alhrtf.dir/examples/alhrtf.c.o: /home/ubuntu/Projects/FinalYearProject/contrib/openal-soft/src/openal-soft/examples/alhrtf.c
CMakeFiles/alhrtf.dir/examples/alhrtf.c.o: CMakeFiles/alhrtf.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/ubuntu/Projects/FinalYearProject/contrib/openal-soft/src/openal-soft-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/alhrtf.dir/examples/alhrtf.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/alhrtf.dir/examples/alhrtf.c.o -MF CMakeFiles/alhrtf.dir/examples/alhrtf.c.o.d -o CMakeFiles/alhrtf.dir/examples/alhrtf.c.o -c /home/ubuntu/Projects/FinalYearProject/contrib/openal-soft/src/openal-soft/examples/alhrtf.c

CMakeFiles/alhrtf.dir/examples/alhrtf.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/alhrtf.dir/examples/alhrtf.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ubuntu/Projects/FinalYearProject/contrib/openal-soft/src/openal-soft/examples/alhrtf.c > CMakeFiles/alhrtf.dir/examples/alhrtf.c.i

CMakeFiles/alhrtf.dir/examples/alhrtf.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/alhrtf.dir/examples/alhrtf.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ubuntu/Projects/FinalYearProject/contrib/openal-soft/src/openal-soft/examples/alhrtf.c -o CMakeFiles/alhrtf.dir/examples/alhrtf.c.s

# Object files for target alhrtf
alhrtf_OBJECTS = \
"CMakeFiles/alhrtf.dir/examples/alhrtf.c.o"

# External object files for target alhrtf
alhrtf_EXTERNAL_OBJECTS =

alhrtf: CMakeFiles/alhrtf.dir/examples/alhrtf.c.o
alhrtf: CMakeFiles/alhrtf.dir/build.make
alhrtf: /usr/lib/x86_64-linux-gnu/libsndfile.so
alhrtf: libex-common.a
alhrtf: libopenal.so.1.23.1
alhrtf: CMakeFiles/alhrtf.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/ubuntu/Projects/FinalYearProject/contrib/openal-soft/src/openal-soft-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable alhrtf"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/alhrtf.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/alhrtf.dir/build: alhrtf
.PHONY : CMakeFiles/alhrtf.dir/build

CMakeFiles/alhrtf.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/alhrtf.dir/cmake_clean.cmake
.PHONY : CMakeFiles/alhrtf.dir/clean

CMakeFiles/alhrtf.dir/depend:
	cd /home/ubuntu/Projects/FinalYearProject/contrib/openal-soft/src/openal-soft-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ubuntu/Projects/FinalYearProject/contrib/openal-soft/src/openal-soft /home/ubuntu/Projects/FinalYearProject/contrib/openal-soft/src/openal-soft /home/ubuntu/Projects/FinalYearProject/contrib/openal-soft/src/openal-soft-build /home/ubuntu/Projects/FinalYearProject/contrib/openal-soft/src/openal-soft-build /home/ubuntu/Projects/FinalYearProject/contrib/openal-soft/src/openal-soft-build/CMakeFiles/alhrtf.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/alhrtf.dir/depend
