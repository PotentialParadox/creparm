cmake_minimum_required(VERSION 3.5)
project(testFitness)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/main.cpp src/fitness.cpp include/fitness.h include/parameter_group.h include/parameters.h include/gaussian_input.h include/gaussian_output.h include/header.h include/coordinates.h include/container_math.h src/coordinates.cpp src/gaussian_input.cpp src/gaussian_output.cpp src/header.cpp src/parameter_group.cpp src/parameters.cpp include/gin_reader.h src/gin_reader.cpp include/gout_reader.h src/gout_reader.cpp)
add_executable(testFitness ${SOURCE_FILES})