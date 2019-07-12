//
// Created by zhsy on 19-6-8.
//

#ifndef HCCSERVER_DEALTHREAD_H
#define HCCSERVER_DEALTHREAD_H

#include <memory>
#include <thread>

class TaskQueue;
class CellTask;



class DealThread {
public:
    typedef std::shared_ptr<TaskQueue> TaskQueuePtr;
    typedef std::shared_ptr<CellTask> CellTaskPtr;
    typedef std::function<void(CellTaskPtr)> WorkFunctor;

    DealThread();
    ~DealThread();
    void startLoop();
    void HandleLoop();
    void setTaskQueue(TaskQueuePtr taskQueue);
    void setMessageCallBack(WorkFunctor workFunctor);
private:
    bool _runing;
    std::thread _t;
    std::shared_ptr<TaskQueue> _taskQueue;
    WorkFunctor _workFunctor;
};

#endif //HCCSERVER_DEALTHREAD_H
