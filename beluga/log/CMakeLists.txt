cmake_minimum_required(VERSION 3.7)
message("message from ./log")

add_library(log_lib SHARED
        LogFile.cpp
        AsynLogging.h
        LogStream.h
        LogStream.cpp
)



