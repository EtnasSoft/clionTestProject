# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.15

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2018.2.5\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2018.2.5\bin\cmake\win\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Dropbox\www\c\clionTestProject

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Dropbox\www\c\clionTestProject\cmake-build-attiny85

# Utility rule file for PLATFORMIO_BUILD_DEBUG.

# Include the progress variables for this target.
include CMakeFiles/PLATFORMIO_BUILD_DEBUG.dir/progress.make

CMakeFiles/PLATFORMIO_BUILD_DEBUG:
	cd /d C:\Dropbox\www\c\clionTestProject && C:\Users\EtnasSoft\.platformio\penv\Scripts\platformio.exe -f -c clion run --target debug -eattiny85

PLATFORMIO_BUILD_DEBUG: CMakeFiles/PLATFORMIO_BUILD_DEBUG
PLATFORMIO_BUILD_DEBUG: CMakeFiles/PLATFORMIO_BUILD_DEBUG.dir/build.make

.PHONY : PLATFORMIO_BUILD_DEBUG

# Rule to build all files generated by this target.
CMakeFiles/PLATFORMIO_BUILD_DEBUG.dir/build: PLATFORMIO_BUILD_DEBUG

.PHONY : CMakeFiles/PLATFORMIO_BUILD_DEBUG.dir/build

CMakeFiles/PLATFORMIO_BUILD_DEBUG.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\PLATFORMIO_BUILD_DEBUG.dir\cmake_clean.cmake
.PHONY : CMakeFiles/PLATFORMIO_BUILD_DEBUG.dir/clean

CMakeFiles/PLATFORMIO_BUILD_DEBUG.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Dropbox\www\c\clionTestProject C:\Dropbox\www\c\clionTestProject C:\Dropbox\www\c\clionTestProject\cmake-build-attiny85 C:\Dropbox\www\c\clionTestProject\cmake-build-attiny85 C:\Dropbox\www\c\clionTestProject\cmake-build-attiny85\CMakeFiles\PLATFORMIO_BUILD_DEBUG.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/PLATFORMIO_BUILD_DEBUG.dir/depend

