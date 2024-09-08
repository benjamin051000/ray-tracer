cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(ray-tracer)

set(CMAKE_CXX_FLAGS
    "--std=c++17 -Ofast -Wall -Wextra -Wunused -Wshadow=compatible-local -Wpedantic -Wconversion" 
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(src)
