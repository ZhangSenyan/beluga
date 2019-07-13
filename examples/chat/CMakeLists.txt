cmake_minimum_required(VERSION 3.7)

add_executable(chatServer server.cpp)
target_link_libraries(chatServer beluga_lib)

add_executable(chatClient client.cpp)
target_link_libraries(chatClient beluga_lib)