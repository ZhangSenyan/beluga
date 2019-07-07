
cmake_minimum_required(VERSION 3.14)

project(HCCServer)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

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

add_executable(HCCServer main.cpp Server.cpp)
target_link_libraries(HCCServer base_lib thread_lib task_lib net_lib Threads::Threads)
#add_executable(HCCServer main.cpp net/Epoll.cpp net/Epoll.h thread/AcceptThread.cpp thread/AcceptThread.h base/MutexLock.h
#         base/noncopyable.h Server.cpp Server.h base/Util.cpp base/Util.h net/Channel.cpp net/Channel.h net/Connection.cpp
#         net/Connection.h base/Buffer.cpp base/Buffer.h base/Timer.cpp base/Timer.h thread/DealThread.cpp thread/DealThread.h
#          task/CellTask.cpp task/CellTask.h task/TaskQueue.h task/TaskQueue.cpp)

#add_executable(client test/client10K.cpp base/Buffer.cpp base/Buffer.h base/Util.cpp base/Util.h net/Connection.cpp net/Connection.h
#        thread/AcceptThread.cpp thread/AcceptThread.h net/Channel.cpp net/Channel.h net/Epoll.cpp net/Epoll.h task/CellTask.h
#        task/CellTask.cpp task/TaskQueue.cpp task/TaskQueue.h Client.cpp Client.h )
#add_executable(TimerTest TimerTest.cpp)

#add_executable(TestThread test/testThread.cpp net/Epoll.cpp net/Epoll.h)

#target_link_libraries(client Threads::Threads)






