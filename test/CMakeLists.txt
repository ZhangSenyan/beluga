cmake_minimum_required(VERSION 3.14)
include_directories(
        ${PROJECT_SOURCE_DIR}/net
        ${PROJECT_SOURCE_DIR}/base
        ${PROJECT_SOURCE_DIR}/task
        ${PROJECT_SOURCE_DIR}/thread
)


add_executable(testMem testMem.cpp ../Server.cpp ../Client.cpp)
target_link_libraries(testMem
        base_lib
        thread_lib
        task_lib
        net_lib
        Threads::Threads
        )