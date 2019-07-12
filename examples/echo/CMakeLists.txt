cmake_minimum_required(VERSION 3.7)

add_executable(echoServer main.cpp)
target_link_libraries(echoServer beluga_lib)

add_executable(echoClient client.cpp)
target_link_libraries(echoClient beluga_lib)