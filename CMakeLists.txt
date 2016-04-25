# 3.1 is needed for the set CXX_STANDARD
cmake_minimum_required (VERSION 3.1)
project(reparm)
set (CMAKE_CXX_STANDARD 14)
set (THREADS_PREFER_PTHREADS_FLAG ON)
find_package(Threads REQUIRED)
# Bring in the headers
include_directories(include)
# Glob the sources since I'm lazy
file(GLOB SOURCES "src/*.cpp")
add_executable(reparm ${SOURCES})
target_link_libraries(reparm ${CMAKE_THREAD_LIBS_INIT})
