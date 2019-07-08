//
// Created by zhsy on 19-7-7.
//

#ifndef HCCSERVER_DEALTHREADPOOL_H
#define HCCSERVER_DEALTHREADPOOL_H

#include <vector>
#include "DealThread.h"
class DealThreadPool {
public:
    DealThreadPool(int size)
    {
        for(int i=0;i<size;i++){
            _threads.emplace_back(new DealThread());
        }
    }
    ~DealThreadPool(){
        
    }
    void setTaskQueue(std::shared_ptr<TaskQueue> taskQueue){
        for(auto thread:_threads){
            thread->setTaskQueue(taskQueue);
        }
    }
    void startLoop(){
        for(auto thread:_threads){
            thread->startLoop();
        }
    }
private:
    std::vector<std::shared_ptr<DealThread>> _threads;
};



#endif //HCCSERVER_DEALTHREADPOOL_H
