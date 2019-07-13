
cmake_minimum_required(VERSION 3.7)

project(beluga)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

set(LIBRARY_OUTPUT_PATH "beluga/lib")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

find_package(Threads REQUIRED)

include_directories(${PROJECT_SOURCE_DIR})

add_subdirectory(beluga)
add_subdirectory(examples)











