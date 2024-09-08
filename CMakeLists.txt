cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(ray-tracer)

set(CMAKE_CXX_FLAGS
    "--std=c++17 -O3 -Wall -Wextra -Wunused -Wshadow=compatible-local -Wpedantic -Wconversion" 
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(src)
