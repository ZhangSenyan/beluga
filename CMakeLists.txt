
cmake_minimum_required(VERSION 3.14)

project(HCCServer)

set(CMAKE_CXX_STANDARD 14)


add_executable(HCCServer main.cpp net/Epoll.cpp net/Epoll.h net/Connection.cpp net/Connection.h util/TimeManager.cpp util/TimeManager.h thread/AcceptThread.cpp thread/AcceptThread.h task/CellTask.cpp task/CellTask.h task/TaskQueue.cpp task/TaskQueue.h thread/ThreadPool.cpp thread/ThreadPool.h thread/DealThread.cpp thread/DealThread.h memory/MemoryPool.cpp memory/MemoryPool.h Server.cpp Server.h util/Util.cpp util/Util.h net/Channel.cpp net/Channel.h)
add_executable(HCCClient Client.cpp)
add_executable(EpollServer epollServer.cpp)







