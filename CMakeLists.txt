# 3.1 is needed for the set CXX_STANDARD
cmake_minimum_required (VERSION 3.1)
set (CMAKE_CXX_STANDARD 11)
project(reparm)
# Bring in the headers
include_directories(include)
# Glob the sources since I'm lazy
file(GLOB SOURCES "src/*.cpp")
add_executable(reparm ${SOURCES})
