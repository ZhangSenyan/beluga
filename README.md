# beluga (a simple C++ network library) 

# Introduction
beluga是一款小型的C++网络库，支持高并发，多线程，用于实现高性能网络服务。
beluga 基于事件驱动，采用非阻塞IO，整体框架由mainReactor,SubReactor,Thread Pool三部分组成。
# Technical points
*基于Reactor模式，包含监听线程，IO线程和计算线程，监听线程负责监听和创建连接，IO线程负责读取写入网络数据，
计算线程负责具体业务逻辑
*IO线程采用EPOLL边沿触发多路复用技术，非阻塞IO
*主线程接收创建连接后以Least Connections方式，分发给IO线程
*采用双任务队列机制，实现紧急任务优先处理
*基于时间轮机制实现心跳检测，剔除超时连接
*为了避免内存泄漏和野指针，重复delete等问题，对于线程间对象使用智能指针等RALL机制
*支持定时定量发送数据
*采用异步日志系统，基于buffer对象池实现日志快速写入
*线程池采用动态倍增扩容机制以及lazy加载模式，最大限度节省内存开销，提高内存申请速率
# Build

# Model

# Log

# HCCServer

1. Buffer的设计
2. 定时定量发送
    基于time_fd定期刷新发送缓冲区，如果到达间隔时间或者
    缓冲区长度超过一半就开始监听写入事件
3. 线程池
4. 内存池，动态扩容机制，扩容过程无需拷贝
5. 双任务队列，实现任务处理优先级
6. 心跳检测，时间轮
7. epoll机制
8. 线程分发,基于小根堆,每次添加连接时首先make_heap
9. 添加任务处理优先级2
10.通过BigBuffer对象池来实现快速日志写入，支持定时定量写入
通过