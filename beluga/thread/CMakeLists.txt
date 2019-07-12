cmake_minimum_required(VERSION 3.7)

message("message from ./thread")

add_library(
        thread_lib SHARED
        #----------------#
        AcceptThread.cpp
        AcceptThread.h
        DealThread.cpp
        DealThread.h
        AcceptThreadPool.cpp
        AcceptThreadPool.h
        EventLoop.cpp
        EventLoop.h
        DealThreadPool.cpp
        DealThreadPool.h
)