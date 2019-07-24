cmake_minimum_required(VERSION 3.7)
message("message from ./net")

add_library(log_lib SHARED
        LogFile.cpp
        AsynLogging.cpp
        LogStream.h
        LogStream.cpp
)



