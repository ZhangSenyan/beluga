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
    AcceptThreadPool(int size,int maxConnSize):_connNum(0),_maxNum(maxConnSize){

        for(int i=0;i<size;i++){
            _threads.emplace_back(new AcceptThread(1.2*maxConnSize/size,50));
        }
    }
    void setTaskQueue(std::shared_ptr<TaskQueue> taskQueue){
        for(auto thread:_threads){
            thread->setTaskQueue(taskQueue);
        }
    }
    int putConnection(std::shared_ptr<Connection> conn){
        if(_connNum>_maxNum)
            return -1;
        std::shared_ptr<AcceptThread> acceptThread(std::move(getMinConnThread()));
        acceptThread->addConnction(std::move(conn));
        conn->setAcceptThread(acceptThread);
        _connNum++;
        return _connNum;
    }
    std::shared_ptr<AcceptThread> getMinConnThread(){
        std::shared_ptr<AcceptThread> minConnThread;
        size_t minConnNum=INT32_MAX;
        for(auto thread:_threads){
            int connNum=thread->getConnSize();
            if(minConnNum>connNum){
                minConnNum=connNum;
                minConnThread=thread;
            }
        }
        return minConnThread;
    }
    void startLoop(){
        for(auto thread:_threads)
            thread->startLoop();
    }
    int size(){
        return _threads.size();
    }
    int maxConnSize(){
        return _maxNum;
    }
    int connSize(){
        return _connNum;
    }
private:
    std::vector<std::shared_ptr<AcceptThread>> _threads;
    size_t _connNum;
    size_t _maxNum;
};


#endif //HCCSERVER_ACCEPTTHREADPOOL_H
