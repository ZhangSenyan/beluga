
cmake_minimum_required(VERSION 3.14)

project(HCCServer)

set(CMAKE_CXX_STANDARD 14)


add_executable(HCCServer Server.cpp Epoll.cpp Epoll.h Connection.cpp Connection.h TimeManager.cpp TimeManager.h AcceptThread.cpp AcceptThread.h CellTask.cpp CellTask.h TaskQueue.cpp TaskQueue.h ThreadPool.cpp ThreadPool.h DealThread.cpp DealThread.h MemoryPool.cpp MemoryPool.h)
add_executable(HCCClient Client.cpp)








