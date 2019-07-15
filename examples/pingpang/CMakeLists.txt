cmake_minimum_required(VERSION 3.7)

add_executable(ppServer ppserver.cpp)
target_link_libraries(ppServer beluga_lib)

add_executable(ppClient ppclient.cpp)
target_link_libraries(ppClient beluga_lib)