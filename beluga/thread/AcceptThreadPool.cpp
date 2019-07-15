//
// Created by zhsy on 19-6-8.
//

#include "AcceptThreadPool.h"
#include <iostream>
AcceptThreadPool::AcceptThreadPool(int size,int maxConnSize):_connNum(0),_maxNum(maxConnSize){

    for(int i=0;i<size;i++){
        _threads.emplace_back(new AcceptThread(static_cast<int>(1.2*maxConnSize/size),50));
    }

}
void AcceptThreadPool::setTaskQueue(const std::shared_ptr<TaskQueue>& taskQueue){
    for(auto& thread:_threads){
        thread->setTaskQueue(taskQueue);
    }
}
int AcceptThreadPool::putConnection(std::shared_ptr<Connection> conn){
    if(_connNum>_maxNum)
        return -1;
    std::shared_ptr<AcceptThread> acceptThread(getMinConnThread());
    conn->setAcceptThread(acceptThread);
    acceptThread->addConnction(conn);
    _connNum++;
    return _connNum;
}
std::shared_ptr<AcceptThread> AcceptThreadPool::getMinConnThread(){
    std::shared_ptr<AcceptThread> minConnThread;
    size_t minConnNum=INT32_MAX;
    for(auto& thread:_threads){
        int connNum=thread->getConnSize();
        if(minConnNum>connNum){
            minConnNum=connNum;
            minConnThread=thread;
        }
    }
    return minConnThread;
}
void AcceptThreadPool::startLoop(){
    for(auto &thread:_threads)
        thread->startLoop();
}
int AcceptThreadPool::size(){
    return _threads.size();
}
int AcceptThreadPool::maxConnSize(){
    return _maxNum;
}
int AcceptThreadPool::connSize(){
    return _connNum;
}
void AcceptThreadPool::setDropConnectionCallBack(beluga::ConnFunctor dropFunctor){
    for(auto &thread:_threads){
        thread->setDropConnectionCallBack(dropFunctor);
    }
}
