cmake_minimum_required(VERSION 3.14)
message("message from ./net")
include_directories(
        ${PROJECT_SOURCE_DIR}/net
        ${PROJECT_SOURCE_DIR}/base
        ${PROJECT_SOURCE_DIR}/task
        ${PROJECT_SOURCE_DIR}/thread
)
add_library(log_lib SHARED
        LogFile.cpp
        LogBuffer.cpp
        ObjectPool.cpp
        AsynLogging.cpp
        LogStream.h
        LogStream.cpp
)



