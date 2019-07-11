cmake_minimum_required(VERSION 3.7)

message("message from ./net")
include_directories(
        ${PROJECT_SOURCE_DIR}/net
        ${PROJECT_SOURCE_DIR}/base
        ${PROJECT_SOURCE_DIR}/task
        ${PROJECT_SOURCE_DIR}/thread
)
add_library(net_lib SHARED Channel.cpp Connection.cpp Epoll.cpp)