# CMakeList.txt : CMake project for ffmx_pack, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("ffmx_pack")

# Add source to this project's executable.
add_executable (ffmx_pack "ffmx_pack.cpp" "ffmx_pack.h")

include_directories("/include")
# TODO: Add tests and install targets if needed.
