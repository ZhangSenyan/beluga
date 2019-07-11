
cmake_minimum_required(VERSION 3.7)

project(beluga)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

set(LIBRARY_OUTPUT_PATH "beluga/lib")
find_package(Threads REQUIRED)

add_subdirectory(beluga/base)
add_subdirectory(beluga/net)
add_subdirectory(beluga/task)
add_subdirectory(beluga/thread)
add_subdirectory(test)
add_subdirectory(beluga/log)

include_directories(
        beluga/net
        beluga/base
        beluga/task
        beluga/thread
        beluga/log
)

add_executable(HCCServer examples/echo/main.cpp beluga/Server.cpp examples/echo/Client.cpp)
target_link_libraries(HCCServer
        base_lib
        thread_lib
        task_lib
        net_lib
        Threads::Threads
)

add_executable(client examples/echo/client10K.cpp beluga/Server.cpp examples/echo/Client.cpp)
target_link_libraries(client
        base_lib
        thread_lib
        task_lib
        net_lib
        Threads::Threads
)
add_executable(LogTest test/LogTest.cpp beluga/Server.cpp examples/echo/Client.cpp)
target_link_libraries(LogTest
        base_lib
        thread_lib
        task_lib
        net_lib
        log_lib
        Threads::Threads
        )







