//
// Created by zhsy on 19-6-8.
// 参考：Jim Fawcett, CSE687 BlockingQueue
//

#ifndef HCCSERVER_TASKQUEUE_H
#define HCCSERVER_TASKQUEUE_H

#include <queue>
#include <pthread.h>
#include <memory>
#include <condition_variable>
#include <mutex>

#include "CellTask.h"
#include "../base/noncopyable.h"

class TaskQueue:noncopyable {
public:
    typedef std::shared_ptr<CellTask> CellTaskPtr;
    TaskQueue(size_t maxSize=100000);
    ~TaskQueue();
    bool push(CellTaskPtr cellTaskPtr);
    CellTaskPtr pop();
    CellTaskPtr front();
    void clear();
    size_t size();
    void reverse(size_t size);
    size_t capacity();
private:
    size_t _maxSize;
    std::queue<CellTaskPtr> _taskQueue;
    std::mutex  _mutex;
    std::condition_variable _cv;
};


#endif //HCCSERVER_TASKQUEUE_H
