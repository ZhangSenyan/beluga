/**
 * @author Zhang Senyan
 * Date: 2019-04-19
 *
 * EventLoop
 */

#ifndef BELUGA_EVENTLOOP_H
#define BELUGA_EVENTLOOP_H

#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <thread>
#include <set>

#include "beluga/base/CurrentThread.h"
#include "beluga/net/Epoll.h"
#include "beluga/base/Timer.h"
#include "beluga/task/TaskQueue.h"
#include "beluga/beluga.h"

class EventLoop {
public:
    /**
     * 构造函数
     * @param size ： 所承载的连接上限
     * @param timer_ms ： 定时间隔
     */
    EventLoop(int size=10000,int timer_ms=50);
    ~EventLoop();

    //启动事件循环
    void startLoop(beluga::LoopMethod method=beluga::LoopMethod::detach);

    //事件循环主体
    void HandleLoop();

    //添加删除事件
    void addChannel(Epoll::ptrChannel channel);
    void removeChannel(Epoll::ptrChannel channel);

    Epoll* getEpoll();

    //定时任务
    virtual void timerHandle();

    bool isInLoopThread();
    void assertInLoopThread();

    //添加待处理事物
    void addPendingFunctor(beluga::CallFunc functor);

    //处理事物
    void doPendingFunctors();

    bool isRuning(){
        return _runing;
    }

    //添加定时任务
    void addTimeFunctor(beluga::CallFunc functor);

private:

    Epoll _epoll;

    Timer _timer;

    //状态位
    bool _runing;

    /**
     * 这里每个eventloop都有自己独立的一个线程
     */
    std::thread _t;

    const std::thread::id _threadId;

    //待处理事务
    std::vector<beluga::CallFunc> _pendingFunctors;

    std::mutex _mutex;

    //定时任务
    std::vector<beluga::CallFunc> _timeFunctors;
};

#endif //BELUGA_EVENTLOOP_H
