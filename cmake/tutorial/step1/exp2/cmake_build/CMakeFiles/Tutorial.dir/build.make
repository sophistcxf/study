# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.20.0/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.20.0/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/xiangfei.cui/workspace/codes/github/study/cmake/tutorial/step1/exp2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/xiangfei.cui/workspace/codes/github/study/cmake/tutorial/step1/exp2/cmake_build

# Include any dependencies generated for this target.
include CMakeFiles/Tutorial.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Tutorial.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Tutorial.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Tutorial.dir/flags.make

CMakeFiles/Tutorial.dir/tutorial.cpp.o: CMakeFiles/Tutorial.dir/flags.make
CMakeFiles/Tutorial.dir/tutorial.cpp.o: ../tutorial.cpp
CMakeFiles/Tutorial.dir/tutorial.cpp.o: CMakeFiles/Tutorial.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/xiangfei.cui/workspace/codes/github/study/cmake/tutorial/step1/exp2/cmake_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Tutorial.dir/tutorial.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Tutorial.dir/tutorial.cpp.o -MF CMakeFiles/Tutorial.dir/tutorial.cpp.o.d -o CMakeFiles/Tutorial.dir/tutorial.cpp.o -c /Users/xiangfei.cui/workspace/codes/github/study/cmake/tutorial/step1/exp2/tutorial.cpp

CMakeFiles/Tutorial.dir/tutorial.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Tutorial.dir/tutorial.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/xiangfei.cui/workspace/codes/github/study/cmake/tutorial/step1/exp2/tutorial.cpp > CMakeFiles/Tutorial.dir/tutorial.cpp.i

CMakeFiles/Tutorial.dir/tutorial.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Tutorial.dir/tutorial.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/xiangfei.cui/workspace/codes/github/study/cmake/tutorial/step1/exp2/tutorial.cpp -o CMakeFiles/Tutorial.dir/tutorial.cpp.s

# Object files for target Tutorial
Tutorial_OBJECTS = \
"CMakeFiles/Tutorial.dir/tutorial.cpp.o"

# External object files for target Tutorial
Tutorial_EXTERNAL_OBJECTS =

Tutorial: CMakeFiles/Tutorial.dir/tutorial.cpp.o
Tutorial: CMakeFiles/Tutorial.dir/build.make
Tutorial: CMakeFiles/Tutorial.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/xiangfei.cui/workspace/codes/github/study/cmake/tutorial/step1/exp2/cmake_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Tutorial"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Tutorial.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Tutorial.dir/build: Tutorial
.PHONY : CMakeFiles/Tutorial.dir/build

CMakeFiles/Tutorial.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Tutorial.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Tutorial.dir/clean

CMakeFiles/Tutorial.dir/depend:
	cd /Users/xiangfei.cui/workspace/codes/github/study/cmake/tutorial/step1/exp2/cmake_build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/xiangfei.cui/workspace/codes/github/study/cmake/tutorial/step1/exp2 /Users/xiangfei.cui/workspace/codes/github/study/cmake/tutorial/step1/exp2 /Users/xiangfei.cui/workspace/codes/github/study/cmake/tutorial/step1/exp2/cmake_build /Users/xiangfei.cui/workspace/codes/github/study/cmake/tutorial/step1/exp2/cmake_build /Users/xiangfei.cui/workspace/codes/github/study/cmake/tutorial/step1/exp2/cmake_build/CMakeFiles/Tutorial.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Tutorial.dir/depend

