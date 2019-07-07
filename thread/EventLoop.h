//
// Created by zhsy on 19-7-7.
//

#ifndef HCCSERVER_EVENTLOOP_H
#define HCCSERVER_EVENTLOOP_H

#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <thread>
#include <set>

#include "Epoll.h"
#include "Timer.h"
#include "TaskQueue.h"
class EventLoop {
public:
    EventLoop(int size=10000,int timer_ms=50);
    ~EventLoop();
    void startLoop();
    void HandleLoop();
    void addChannel(Epoll::ptrChannel channel);
    void removeChannel(Epoll::ptrChannel channel);
    Epoll* getEpoll();
    virtual void timerHandle();
    bool isInLoopThread();
    void assertInLoopThread();

private:
    Epoll _epoll;
    Timer _timer;
    bool _runing;
    std::thread _t;
};


#endif //HCCSERVER_EVENTLOOP_H
