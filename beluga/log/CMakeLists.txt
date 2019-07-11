cmake_minimum_required(VERSION 3.7)
message("message from ./net")
include_directories(
        ../net
        ../base
        ../task
        ../thread
)
add_library(log_lib SHARED
        LogFile.cpp
        LogBuffer.cpp
        ObjectPool.cpp
        AsynLogging.cpp
        LogStream.h
        LogStream.cpp
)



