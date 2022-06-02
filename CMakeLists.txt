cmake_minimum_required(VERSION 3.22)
project(matrix_multiplication)

set(CMAKE_CXX_STANDARD 20)
find_package(OpenMP REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

add_executable(matrix_multiplication main.cpp)
