//
// Created by zhsy on 19-7-7.
//

#include "DealThreadPool.h"

/*
 * 初始化创建线程池，大小为size，放到vector中
 */
DealThreadPool::DealThreadPool(int size):_threads()
{
    for(int i=0;i<size;i++){
        _threads.emplace_back(new DealThread());
    }
}

DealThreadPool::~DealThreadPool(){

}
/*
 * 设置任务队列
 */
void DealThreadPool::setTaskQueue(std::shared_ptr<TaskQueue> taskQueue){
    for(auto thread:_threads){
        thread->setTaskQueue(taskQueue);
    }
}
/*
 * 线程池启动
 */
void DealThreadPool::startLoop(){
    for(auto thread:_threads){
        thread->startLoop();
    }
}
void DealThreadPool::setMessageCallBack(DealThread::WorkFunctor workFunctor){
    for(auto thread:_threads){
        thread->setMessageCallBack(workFunctor);
    }
}