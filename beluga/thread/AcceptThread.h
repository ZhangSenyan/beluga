//
// Created by zhsy on 19-6-8.
//

#ifndef HCCSERVER_ACCEPTTHREAD_H
#define HCCSERVER_ACCEPTTHREAD_H


#include <iostream>
#include <map>

#include "beluga/net/Connection.h"
#include "beluga/task/TaskQueue.h"
#include "beluga/base/TimingWheel.h"
#include "beluga/thread/EventLoop.h"

class AcceptThread :public EventLoop{
public:
    AcceptThread(int size=10000,int timer_ms=50);
    ~AcceptThread();
    void addConnction(std::shared_ptr<Connection> conn);
    void removeConnction(std::shared_ptr<Connection> conn);
    void removeConnction(int fd);
    void timerHandle() override ;
    void timerHandle50();
    void timerHandle1000();
    void setTaskQueue(const std::shared_ptr<TaskQueue>& taskQueue);
    std::shared_ptr<TaskQueue> getTaskQueue();
    std::map<int ,std::shared_ptr<Connection>>& getConnMap();
    int getConnSize();
    int updateConn(Connection::ConnPtr conn);
private:

    std::map<int,std::shared_ptr<Connection>> connMap;
    std::shared_ptr<TaskQueue> _taskQueue;
    TimingWheel _timingWheel;
    int _timeCount;
    
};


#endif //HCCSERVER_ACCEPTTHREAD_H
