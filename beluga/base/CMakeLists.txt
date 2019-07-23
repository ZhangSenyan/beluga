cmake_minimum_required(VERSION 3.7)

message("message from ./base")

add_library(
        base_lib SHARED
        #----------------#
        Buffer.cpp
        Timer.cpp
        Util.cpp
        TimingWheel.cpp
        TimingWheel.h
        Basket.h
        MemoryPool.cpp
        MemoryPool.h
        MemoryBlock.cpp
        MemoryBlock.h
        Alloctor.h
        Alloctor.cpp
        CountDownLatch.cc
)


