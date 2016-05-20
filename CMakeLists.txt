# 3.1 is needed for the set CXX_STANDARD
cmake_minimum_required (VERSION 3.1)
project(reparm)
set (CMAKE_CXX_STANDARD 14)
set (GCC_FLAGS "-g")
set (THREADS_PREFER_PTHREADS_FLAG ON)
set (CMAKE_EXPORT_COMPILE_COMMANDS 1)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_FLAGS}")
find_package(Threads REQUIRED)
find_package(Armadillo REQUIRED)
# Bring in the headers
include_directories(include)
# Glob the sources since I'm lazy
file(GLOB SOURCES "src/*.cpp")
add_executable(reparm ${SOURCES})
target_link_libraries(reparm ${CMAKE_THREAD_LIBS_INIT})
