//
// Created by zhsy on 19-6-8.
//

#ifndef HCCSERVER_DEALTHREAD_H
#define HCCSERVER_DEALTHREAD_H

#include <thread>
class TaskQueue;
class CellTask;

//.cpp
#include <iostream>
class DealThread {
public:
    typedef std::shared_ptr<TaskQueue> TaskQueuePtr;
    typedef std::shared_ptr<CellTask> CellTaskPtr;

    DealThread();
    ~DealThread();
    void startLoop();
    void HandleLoop();
    void setTaskQueue(TaskQueuePtr taskQueue);

private:
    bool _runing;
    std::thread _t;
    std::shared_ptr<TaskQueue> _taskQueue;
};

#endif //HCCSERVER_DEALTHREAD_H
