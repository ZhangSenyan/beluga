cmake_minimum_required(VERSION 3.7)

add_subdirectory(thread)
add_subdirectory(base)
add_subdirectory(log)
add_subdirectory(net)
add_subdirectory(task)



add_library(beluga_lib SHARED TCPServer.cpp Client.cpp)
target_link_libraries(beluga_lib
        base_lib
        thread_lib
        task_lib
        net_lib
        log_lib
        Threads::Threads
)