cmake_minimum_required(VERSION 3.5)
project(testFitness)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set (GCC_FLAGS "-g")
set (THREADS_PREFER_PTHREADS_FLAG ON)
set (CMAKE_EXPORT_COMPILE_COMMANDS 1)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_FLAGS}")
find_package(Threads REQUIRED)
find_package(Armadillo REQUIRED)
# Bring in the headers
include_directories(include ${ARMADILLO_INCLUDE_DIRS})

set(SOURCE_FILES src/main.cpp src/fitness.cpp include/fitness.h include/parameter_group.h include/parameters.h include/gaussian_input.h include/gaussian_output.h include/header.h include/coordinates.h include/container_math.h src/coordinates.cpp src/gaussian_input.cpp src/gaussian_output.cpp src/header.cpp src/parameter_group.cpp src/parameters.cpp include/gin_reader.h src/gin_reader.cpp include/gout_reader.h src/gout_reader.cpp include/reparm_data.h include/reparm_input.h src/reparm_data.cpp src/reparm_input.cpp include/system_tools.h include/gaussian.h src/gaussian.cpp)
add_executable(testFitness ${SOURCE_FILES})
target_link_libraries(testFitness ${CMAKE_THREAD_LIBS_INIT} ${ARMADILLO_LIBRARIES})
