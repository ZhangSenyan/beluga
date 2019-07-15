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

#include "beluga/base/CurrentThread.h"
#include "beluga/net/Epoll.h"
#include "beluga/base/Timer.h"
#include "beluga/task/TaskQueue.h"

class EventLoop {
public:
    typedef std::function<void()> Functor;
    EventLoop(int size=10000,int timer_ms=50);
    ~EventLoop();
    void startLoop(beluga::LoopMethod method=beluga::LoopMethod::detach);
    void HandleLoop();
    void addChannel(Epoll::ptrChannel channel);
    void removeChannel(Epoll::ptrChannel channel);
    Epoll* getEpoll();
    virtual void timerHandle();
    bool isInLoopThread();
    void assertInLoopThread();
    void addPendingFunctor(Functor functor);
    void doPendingFunctors();
    bool isRuning(){
        return _runing;
    }
    void addTimeFunctor(Functor functor);
private:
    Epoll _epoll;
    Timer _timer;
    bool _runing;
    std::thread _t;
    const std::thread::id _threadId;
    std::vector<Functor> _pendingFunctors;
    std::mutex _mutex;
    std::vector<Functor> _timeFunctors;
};

#endif //HCCSERVER_EVENTLOOP_H
