//
// Created by zhsy on 19-6-8.
//

#ifndef HCCSERVER_ACCEPTTHREAD_H
#define HCCSERVER_ACCEPTTHREAD_H


#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <thread>
#include <set>

#include "../net/Epoll.h"
#include "../base/Timer.h"
#include "../task/TaskQueue.h"

class AcceptThread {
public:
    AcceptThread(int size=10000,int timer_ms=50);
    ~AcceptThread();
    void startLoop();
    void HandleLoop();
    void addChannel(Epoll::ptrChannel channel);
    void addConnction(std::shared_ptr<Connection> conn);
    std::set<std::shared_ptr<Connection>>& getConnSet();
    void flushBuffer();
    Epoll* getEpoll();
    void setTaskQueue(std::shared_ptr<TaskQueue> taskQueue);
    std::shared_ptr<TaskQueue> getTaskQueue();
private:
    Epoll _epoll;
    Timer _timer;
    bool _runing;
    std::thread _t;
    std::set<std::shared_ptr<Connection>> connSet;
    std::shared_ptr<TaskQueue> _taskQueue;
};


#endif //HCCSERVER_ACCEPTTHREAD_H
