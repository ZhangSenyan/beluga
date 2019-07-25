/**
 * @author Zhang Senyan
 * Date: 2019-04-29
 *
 * 计算线程池
 */

#include "DealThreadPool.h"

using namespace beluga;


/**
 * 构造函数
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

/**
 * 设置任务队列
 */
void DealThreadPool::setTaskQueue(std::shared_ptr<TaskQueue> taskQueue){
    for(auto thread:_threads){
        thread->setTaskQueue(taskQueue);
    }
}

/**
 * 线程池启动
 */
void DealThreadPool::startLoop(){
    for(auto thread:_threads){
        thread->startLoop();
    }
}

/**
 * 消息接收回显
 * @param workFunctor
 */
void DealThreadPool::setMessageCallBack(WorkFunctor workFunctor){
    for(auto thread:_threads){
        thread->setMessageCallBack(workFunctor);
    }
}