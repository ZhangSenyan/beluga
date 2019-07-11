cmake_minimum_required(VERSION 3.7)

message("message from ./thread")
include_directories(
        ../net
        ../base
        ../task
        .
)


add_library(thread_lib SHARED AcceptThread.cpp DealThread.cpp AcceptThreadPool.cpp EventLoop.cpp EventLoop.h DealThreadPool.cpp DealThreadPool.h)