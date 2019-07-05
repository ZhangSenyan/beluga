# HCCServer
A High Concurrency Server
1. Buffer的设计
2. 定时定量发送

    基于time_fd定期刷新发送缓冲区，如果到达间隔时间或者
    缓冲区长度超过一半就开始监听写入事件
3. 线程池
4. 内存池
5. 消息队列
6. 心跳检测
7. epoll机制
8. 线程分发,基于小根堆,每次添加连接时首先make_heap
9. 添加任务处理优先级2