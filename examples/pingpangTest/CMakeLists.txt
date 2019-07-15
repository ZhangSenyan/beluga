cmake_minimum_required(VERSION 3.7)

add_executable(ppServerTest ppserverTest.cpp)
target_link_libraries(ppServerTest beluga_lib)

add_executable(ppClientTest ppclientTest.cpp)
target_link_libraries(ppClientTest beluga_lib)