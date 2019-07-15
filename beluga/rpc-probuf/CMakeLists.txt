cmake_minimum_required(VERSION 3.7)
message("message from ./net")

find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})

add_library(
        rpc_lib SHARED

        RPC_Channel.cpp
        RPC_Channel.h
        RPC_Server.cpp
        RPC_Server.h
        beluga.pb.cc
)
target_link_libraries(
        rpc_lib
        log_lib
        ${Protobuf_LIBRARIES}
)


