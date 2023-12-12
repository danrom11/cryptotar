# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/asd/kurs/hub/cryptotar

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/asd/kurs/hub/cryptotar

# Include any dependencies generated for this target.
include CMakeFiles/cryptotar.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/cryptotar.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/cryptotar.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/cryptotar.dir/flags.make

CMakeFiles/cryptotar.dir/cryptotarlib.cpp.o: CMakeFiles/cryptotar.dir/flags.make
CMakeFiles/cryptotar.dir/cryptotarlib.cpp.o: cryptotarlib.cpp
CMakeFiles/cryptotar.dir/cryptotarlib.cpp.o: CMakeFiles/cryptotar.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/asd/kurs/hub/cryptotar/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/cryptotar.dir/cryptotarlib.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/cryptotar.dir/cryptotarlib.cpp.o -MF CMakeFiles/cryptotar.dir/cryptotarlib.cpp.o.d -o CMakeFiles/cryptotar.dir/cryptotarlib.cpp.o -c /home/asd/kurs/hub/cryptotar/cryptotarlib.cpp

CMakeFiles/cryptotar.dir/cryptotarlib.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/cryptotar.dir/cryptotarlib.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/asd/kurs/hub/cryptotar/cryptotarlib.cpp > CMakeFiles/cryptotar.dir/cryptotarlib.cpp.i

CMakeFiles/cryptotar.dir/cryptotarlib.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/cryptotar.dir/cryptotarlib.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/asd/kurs/hub/cryptotar/cryptotarlib.cpp -o CMakeFiles/cryptotar.dir/cryptotarlib.cpp.s

CMakeFiles/cryptotar.dir/sha256.cpp.o: CMakeFiles/cryptotar.dir/flags.make
CMakeFiles/cryptotar.dir/sha256.cpp.o: sha256.cpp
CMakeFiles/cryptotar.dir/sha256.cpp.o: CMakeFiles/cryptotar.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/asd/kurs/hub/cryptotar/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/cryptotar.dir/sha256.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/cryptotar.dir/sha256.cpp.o -MF CMakeFiles/cryptotar.dir/sha256.cpp.o.d -o CMakeFiles/cryptotar.dir/sha256.cpp.o -c /home/asd/kurs/hub/cryptotar/sha256.cpp

CMakeFiles/cryptotar.dir/sha256.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/cryptotar.dir/sha256.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/asd/kurs/hub/cryptotar/sha256.cpp > CMakeFiles/cryptotar.dir/sha256.cpp.i

CMakeFiles/cryptotar.dir/sha256.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/cryptotar.dir/sha256.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/asd/kurs/hub/cryptotar/sha256.cpp -o CMakeFiles/cryptotar.dir/sha256.cpp.s

# Object files for target cryptotar
cryptotar_OBJECTS = \
"CMakeFiles/cryptotar.dir/cryptotarlib.cpp.o" \
"CMakeFiles/cryptotar.dir/sha256.cpp.o"

# External object files for target cryptotar
cryptotar_EXTERNAL_OBJECTS =

libcryptotar.a: CMakeFiles/cryptotar.dir/cryptotarlib.cpp.o
libcryptotar.a: CMakeFiles/cryptotar.dir/sha256.cpp.o
libcryptotar.a: CMakeFiles/cryptotar.dir/build.make
libcryptotar.a: CMakeFiles/cryptotar.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/asd/kurs/hub/cryptotar/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX static library libcryptotar.a"
	$(CMAKE_COMMAND) -P CMakeFiles/cryptotar.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cryptotar.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/cryptotar.dir/build: libcryptotar.a
.PHONY : CMakeFiles/cryptotar.dir/build

CMakeFiles/cryptotar.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/cryptotar.dir/cmake_clean.cmake
.PHONY : CMakeFiles/cryptotar.dir/clean

CMakeFiles/cryptotar.dir/depend:
	cd /home/asd/kurs/hub/cryptotar && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/asd/kurs/hub/cryptotar /home/asd/kurs/hub/cryptotar /home/asd/kurs/hub/cryptotar /home/asd/kurs/hub/cryptotar /home/asd/kurs/hub/cryptotar/CMakeFiles/cryptotar.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/cryptotar.dir/depend

