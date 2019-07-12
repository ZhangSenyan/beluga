cmake_minimum_required(VERSION 3.7)

message("message from ./task")

add_library(task_lib SHARED CellTask.cpp TaskQueue.cpp)