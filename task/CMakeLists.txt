cmake_minimum_required(VERSION 3.14)

message("message from ./task")
include_directories(
        ${PROJECT_SOURCE_DIR}/net
        ${PROJECT_SOURCE_DIR}/base
        ${PROJECT_SOURCE_DIR}/task
        ${PROJECT_SOURCE_DIR}/thread
)

add_library(task_lib SHARED CellTask.cpp TaskQueue.cpp)