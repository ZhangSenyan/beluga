//
// Created by zhsy on 19-6-8.
//

#ifndef HCCSERVER_ACCEPTTHREAD_H
#define HCCSERVER_ACCEPTTHREAD_H


#include <iostream>
//#include <pthread.h>
//#include <unistd.h>
//#include <thread>
//#include <set>

#include "EventLoop.h"
#include "TaskQueue.h"

class AcceptThread :public EventLoop{
public:
    AcceptThread(int size=10000,int timer_ms=50);
    ~AcceptThread();

    void addConnction(std::shared_ptr<Connection> conn);

    void timerHandle() override ;

    void setTaskQueue(std::shared_ptr<TaskQueue> taskQueue);
    std::shared_ptr<TaskQueue> getTaskQueue();
    std::set<std::shared_ptr<Connection>>& getConnSet();
    int getConnSize();
private:

    std::set<std::shared_ptr<Connection>> connSet;
    std::shared_ptr<TaskQueue> _taskQueue;
    
};


#endif //HCCSERVER_ACCEPTTHREAD_H

