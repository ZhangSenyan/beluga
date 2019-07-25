/**
 * @author Zhang Senyan
 * Date: 2019-05-05
 *
 * 任务队列
 * @refitem Jim Fawcett, CSE687 BlockingQueue
 */


#ifndef BELUGA_TASKQUEUE_H
#define BELUGA_TASKQUEUE_H

#include <queue>
#include <pthread.h>
#include <memory>
#include <condition_variable>
#include <mutex>

#include "beluga/task/CellTask.h"
#include "beluga/base/noncopyable.h"

class TaskQueue:noncopyable {
public:
    typedef std::shared_ptr<CellTask> CellTaskPtr;

    /**
     * 构造函数
     * @param maxSize：任务队列最大长度
     */
    explicit TaskQueue(size_t maxSize=100000);
    ~TaskQueue();


    bool push(CellTaskPtr cellTaskPtr);

    CellTaskPtr pop();

    CellTaskPtr front();

    void clear();

    size_t size();

    //重置队列容量大小
    void reverse(size_t size);

    //获取队列最大容量
    size_t capacity();

private:

    size_t _maxSize;

    std::queue<CellTaskPtr> _taskQueue;

    std::mutex  _mutex;

    std::condition_variable _cv;
};


#endif //BELUGA_TASKQUEUE_H
