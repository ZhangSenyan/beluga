//
// Created by zhsy on 19-6-8.
//

#ifndef HCCSERVER_ACCEPTTHREADPOOL_H
#define HCCSERVER_ACCEPTTHREADPOOL_H

#include <cstddef>
#include <vector>
#include <memory>

#include "AcceptThread.h"
#include "Connection.h"
class AcceptThreadPool {
public:
    AcceptThreadPool(int size,int maxConnSize);
    void setTaskQueue(const std::shared_ptr<TaskQueue>& taskQueue);
    int putConnection(std::shared_ptr<Connection> conn);
    std::shared_ptr<AcceptThread> getMinConnThread();
    void startLoop();
    int size();
    int maxConnSize();
    int connSize();
private:
    std::vector<std::shared_ptr<AcceptThread>> _threads;
    size_t _connNum;
    size_t _maxNum;
};



#endif //HCCSERVER_ACCEPTTHREADPOOL_H
