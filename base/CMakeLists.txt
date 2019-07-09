cmake_minimum_required(VERSION 3.14)

message("message from ./base")
include_directories(
        ${PROJECT_SOURCE_DIR}/net
        ${PROJECT_SOURCE_DIR}/base
        ${PROJECT_SOURCE_DIR}/task
        ${PROJECT_SOURCE_DIR}/thread
)
add_library(
        base_lib SHARED
        #----------------#
        Buffer.cpp
        Timer.cpp
        Util.cpp
        TimingWheel.cpp
        TimingWheel.h
        Basket.cpp
        Basket.h
        MemoryPool.cpp
        MemoryPool.h
        MemoryBlock.cpp
        MemoryBlock.h
        Alloctor.h
        Alloctor.cpp
)
add_subdirectory(test)