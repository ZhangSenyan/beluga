cmake_minimum_required(VERSION 3.7)

add_executable(rpcServer
        easyRpcServer.cpp
        protobuf/echo.pb.cc
        )
find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
target_link_libraries(
        rpcServer
        #-------------#
        beluga_lib
        ${Protobuf_LIBRARIES}
)

add_executable(rpcClient
        easyRpcClient.cpp
        protobuf/echo.pb.cc
        )

target_link_libraries(
        rpcClient
        #-------------#
        beluga_lib
        ${Protobuf_LIBRARIES}
)



