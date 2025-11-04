cmake_minimum_required(VERSION 3.16)

set(projectName "Tree_Growth")

message(STATUS "Starting Build...")
project(${projectName})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
add_subdirectory(main)