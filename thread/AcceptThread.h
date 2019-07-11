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
#include "TimingWheel.h"
class AcceptThread :public EventLoop{
public:
    AcceptThread(int size=10000,int timer_ms=50);
    ~AcceptThread();

    void addConnction(std::shared_ptr<Connection>& conn);
    void removeConnction(std::shared_ptr<Connection>& conn);
    void removeConnction(int fd);
    void timerHandle() override ;
    void timerHandle50();
    void timerHandle1000();
    void setTaskQueue(const std::shared_ptr<TaskQueue>& taskQueue);
    std::shared_ptr<TaskQueue> getTaskQueue();
    std::map<int ,std::shared_ptr<Connection>>& getConnMap();
    int getConnSize();
    int updateConn(Connection::ConnPtr& conn);
private:

    std::map<int,std::shared_ptr<Connection>> connMap;
    std::shared_ptr<TaskQueue> _taskQueue;
    TimingWheel _timingWheel;
    int _timeCount;
    
};


#endif //HCCSERVER_ACCEPTTHREAD_H
