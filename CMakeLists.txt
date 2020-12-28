# CMakeList.txt : CMake project for myUtil, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("myUtil")

# Add source to this project's executable.
add_executable (myUtil "myUtil.cpp" "myUtil.h" "ffmpeg/writeFile.hpp")

# TODO: Add tests and install targets if needed.
