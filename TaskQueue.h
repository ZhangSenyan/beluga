//
// Created by zhsy on 19-6-8.
//

#ifndef HCCSERVER_TASKQUEUE_H
#define HCCSERVER_TASKQUEUE_H

#include <queue>
#include <pthread.h>

template <class Object>
class TaskQueue {
public:
    TaskQueue();
    ~TaskQueue();
    bool enter(Object);
    Object out();

private:
    int _maxSize;
    std::queue<Object> base;
    pthread_mutex_t queueMutex;

};


#endif //HCCSERVER_TASKQUEUE_H
