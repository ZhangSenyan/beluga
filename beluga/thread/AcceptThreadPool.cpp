/**
 * @author Zhang Senyan
 * Date: 2019-04-25
 *
 */

#include "AcceptThreadPool.h"
#include <iostream>

/**
 * 构造函数
 */
AcceptThreadPool::AcceptThreadPool(int size,int maxConnSize):
                    _connNum(0),
                    _maxNum(maxConnSize){

    /**
     * 1.2*maxConnSize/size
     * 将线程池连接上限均分到各个线程池
     * 并保留1.2倍余量，因为负载不一定完全均衡
     */
    for(int i=0;i<size;i++){
        _threads.emplace_back(new AcceptThread(static_cast<int>(1.2*maxConnSize/size),50));
    }

}

/**
 * 设定任务队列 ： IO线程 -> 计算线程
 * @param taskQueue
 */
void AcceptThreadPool::setTaskQueue(const std::shared_ptr<TaskQueue>& taskQueue){
    for(auto& thread:_threads){
        thread->setTaskQueue(taskQueue);
    }
}

/**
 * 添加一个新的客户端连接
 * @param conn
 * @return
 */
int AcceptThreadPool::putConnection(std::shared_ptr<Connection> conn){

    //上限检测
    if(_connNum>_maxNum)
        return -1;

    //得到当前负载数最少的线程
    std::shared_ptr<AcceptThread> acceptThread(getMinConnThread());
    conn->setAcceptThread(acceptThread);

    //将该连接丢给该线程
    acceptThread->addConnction(conn);
    _connNum++;
    return _connNum;
}

/**
 * 获得当前负载最小的线程
 * @return AcceptThread ptr
 */
std::shared_ptr<AcceptThread> AcceptThreadPool::getMinConnThread(){

    std::shared_ptr<AcceptThread> minConnThread;
    size_t minConnNum=INT32_MAX;

    //遍历查找
    for(auto& thread:_threads){
        int connNum=thread->getConnSize();
        if(minConnNum>connNum){
            minConnNum=connNum;
            minConnThread=thread;
        }
    }
    return minConnThread;
}

/**
 * 启动线程池
 */
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

/**
 * 设置掉线处理函数
 * @param dropFunctor
 */
void AcceptThreadPool::setDropConnectionCallBack(beluga::ConnFunctor dropFunctor){
    for(auto &thread:_threads){
        thread->setDropConnectionCallBack(dropFunctor);
    }
}
