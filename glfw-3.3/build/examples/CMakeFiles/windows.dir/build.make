# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.24

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
CMAKE_COMMAND = /opt/local/bin/cmake

# The command to remove a file.
RM = /opt/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/stanislavromanov/CLionProjects/openGL_App/glfw-3.3

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/stanislavromanov/CLionProjects/openGL_App/glfw-3.3/build

# Include any dependencies generated for this target.
include examples/CMakeFiles/windows.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include examples/CMakeFiles/windows.dir/compiler_depend.make

# Include the progress variables for this target.
include examples/CMakeFiles/windows.dir/progress.make

# Include the compile flags for this target's objects.
include examples/CMakeFiles/windows.dir/flags.make

examples/windows.app/Contents/Resources/glfw.icns: /Users/stanislavromanov/CLionProjects/openGL_App/glfw-3.3/examples/glfw.icns
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Copying OS X content examples/windows.app/Contents/Resources/glfw.icns"
	$(CMAKE_COMMAND) -E copy /Users/stanislavromanov/CLionProjects/openGL_App/glfw-3.3/examples/glfw.icns examples/windows.app/Contents/Resources/glfw.icns

examples/CMakeFiles/windows.dir/windows.c.o: examples/CMakeFiles/windows.dir/flags.make
examples/CMakeFiles/windows.dir/windows.c.o: /Users/stanislavromanov/CLionProjects/openGL_App/glfw-3.3/examples/windows.c
examples/CMakeFiles/windows.dir/windows.c.o: examples/CMakeFiles/windows.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/stanislavromanov/CLionProjects/openGL_App/glfw-3.3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object examples/CMakeFiles/windows.dir/windows.c.o"
	cd /Users/stanislavromanov/CLionProjects/openGL_App/glfw-3.3/build/examples && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT examples/CMakeFiles/windows.dir/windows.c.o -MF CMakeFiles/windows.dir/windows.c.o.d -o CMakeFiles/windows.dir/windows.c.o -c /Users/stanislavromanov/CLionProjects/openGL_App/glfw-3.3/examples/windows.c

examples/CMakeFiles/windows.dir/windows.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/windows.dir/windows.c.i"
	cd /Users/stanislavromanov/CLionProjects/openGL_App/glfw-3.3/build/examples && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/stanislavromanov/CLionProjects/openGL_App/glfw-3.3/examples/windows.c > CMakeFiles/windows.dir/windows.c.i

examples/CMakeFiles/windows.dir/windows.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/windows.dir/windows.c.s"
	cd /Users/stanislavromanov/CLionProjects/openGL_App/glfw-3.3/build/examples && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/stanislavromanov/CLionProjects/openGL_App/glfw-3.3/examples/windows.c -o CMakeFiles/windows.dir/windows.c.s

# Object files for target windows
windows_OBJECTS = \
"CMakeFiles/windows.dir/windows.c.o"

# External object files for target windows
windows_EXTERNAL_OBJECTS =

examples/windows.app/Contents/MacOS/windows: examples/CMakeFiles/windows.dir/windows.c.o
examples/windows.app/Contents/MacOS/windows: examples/CMakeFiles/windows.dir/build.make
examples/windows.app/Contents/MacOS/windows: src/libglfw3.a
examples/windows.app/Contents/MacOS/windows: examples/CMakeFiles/windows.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/stanislavromanov/CLionProjects/openGL_App/glfw-3.3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable windows.app/Contents/MacOS/windows"
	cd /Users/stanislavromanov/CLionProjects/openGL_App/glfw-3.3/build/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/windows.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/CMakeFiles/windows.dir/build: examples/windows.app/Contents/MacOS/windows
examples/CMakeFiles/windows.dir/build: examples/windows.app/Contents/Resources/glfw.icns
.PHONY : examples/CMakeFiles/windows.dir/build

examples/CMakeFiles/windows.dir/clean:
	cd /Users/stanislavromanov/CLionProjects/openGL_App/glfw-3.3/build/examples && $(CMAKE_COMMAND) -P CMakeFiles/windows.dir/cmake_clean.cmake
.PHONY : examples/CMakeFiles/windows.dir/clean

examples/CMakeFiles/windows.dir/depend:
	cd /Users/stanislavromanov/CLionProjects/openGL_App/glfw-3.3/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/stanislavromanov/CLionProjects/openGL_App/glfw-3.3 /Users/stanislavromanov/CLionProjects/openGL_App/glfw-3.3/examples /Users/stanislavromanov/CLionProjects/openGL_App/glfw-3.3/build /Users/stanislavromanov/CLionProjects/openGL_App/glfw-3.3/build/examples /Users/stanislavromanov/CLionProjects/openGL_App/glfw-3.3/build/examples/CMakeFiles/windows.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/CMakeFiles/windows.dir/depend

