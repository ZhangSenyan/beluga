//
// Created by zhsy on 19-7-7.
//

#ifndef HCCSERVER_DEALTHREADPOOL_H
#define HCCSERVER_DEALTHREADPOOL_H

#include <vector>
#include "DealThread.h"
class DealThreadPool {
public:
    DealThreadPool(int size);
    ~DealThreadPool();
    void setTaskQueue(std::shared_ptr<TaskQueue> taskQueue);
    void startLoop();
    void setMessageCallBack(DealThread::WorkFunctor workFunctor);
private:
    std::vector<std::shared_ptr<DealThread>> _threads;
};

#endif //HCCSERVER_DEALTHREADPOOL_H
