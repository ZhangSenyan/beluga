
cmake_minimum_required(VERSION 3.14)

project(HCCServer)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/lib")
find_package(Threads REQUIRED)

add_subdirectory(base)
add_subdirectory(net)
add_subdirectory(task)
add_subdirectory(thread)

include_directories(
        ${PROJECT_SOURCE_DIR}/net
        ${PROJECT_SOURCE_DIR}/base
        ${PROJECT_SOURCE_DIR}/task
        ${PROJECT_SOURCE_DIR}/thread
)

add_executable(HCCServer main.cpp Server.cpp Client.cpp)
target_link_libraries(HCCServer
        base_lib
        thread_lib
        task_lib
        net_lib
        Threads::Threads
)

add_executable(client client10K.cpp Server.cpp Client.cpp)
target_link_libraries(client
        base_lib
        thread_lib
        task_lib
        net_lib
        Threads::Threads
)








