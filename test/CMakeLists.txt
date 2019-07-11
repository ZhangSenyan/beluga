cmake_minimum_required(VERSION 3.7)
include_directories(
        ../beluga/net
        ../beluga/base
        ../beluga/task
        ../beluga/thread
)


add_executable(testMem testMem.cpp ../beluga/Server.cpp ../examples/echo/Client.cpp)
target_link_libraries(testMem
        base_lib
        thread_lib
        task_lib
        net_lib
        Threads::Threads
        )