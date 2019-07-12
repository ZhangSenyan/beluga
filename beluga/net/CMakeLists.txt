cmake_minimum_required(VERSION 3.7)

message("message from ./net")

add_library(
        net_lib SHARED
        Channel.cpp
        Connection.cpp
        Epoll.cpp
)