cmake_minimum_required(VERSION 3.14)

message("message from ./thread")
include_directories(
        ${PROJECT_SOURCE_DIR}/net
        ${PROJECT_SOURCE_DIR}/base
        ${PROJECT_SOURCE_DIR}/task
        ${PROJECT_SOURCE_DIR}/thread
)

add_library(thread_lib SHARED AcceptThread.cpp DealThread.cpp AcceptThreadPool.cpp EventLoop.cpp EventLoop.h DealThreadPool.cpp DealThreadPool.h)