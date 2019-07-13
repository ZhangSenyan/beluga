//
// Created by zhsy on 19-6-8.
//

#ifndef HCCSERVER_DEALTHREAD_H
#define HCCSERVER_DEALTHREAD_H

#include <memory>
#include <thread>
#include "beluga/beluga.h"
class TaskQueue;
class CellTask;




class DealThread {
public:

    DealThread();
    ~DealThread();
    void startLoop();
    void HandleLoop();
    void setTaskQueue(beluga::TaskQueuePtr taskQueue);
    void setMessageCallBack(beluga::WorkFunctor workFunctor);
private:
    bool _runing;
    std::thread _t;
    std::shared_ptr<TaskQueue> _taskQueue;
    beluga::WorkFunctor _workFunctor;
};

#endif //HCCSERVER_DEALTHREAD_H
