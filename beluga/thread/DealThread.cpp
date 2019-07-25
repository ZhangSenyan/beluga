/**
 * @author Zhang Senyan
 * Date: 2019-04-29
 *
 */

#include <string>
#include <iostream>
#include <memory>
#include <thread>
#include "unistd.h"

#include "beluga/task/CellTask.h"
#include "beluga/task/TaskQueue.h"

#include "DealThread.h"


using namespace beluga;

/**
 * 构造函数
 */
DealThread::DealThread():
        _runing(false),
        _t(std::mem_fun(&DealThread::HandleLoop),this),
        _taskQueue(nullptr){

    std::cout<<"DealThread::DealThread"<<std::endl;
}
DealThread::~DealThread(){

}

/**
 * 启动计算线程
 */
void DealThread::startLoop(){
    _runing=true;
    _t.detach();
}


void DealThread::HandleLoop(){

    /**
     * 待完善
     * 改为countdownlatch
     */
    while(!_runing) usleep(10*1000);

    while(_runing){

        //监听任务
        if(_taskQueue.get()){

            //处理任务
            _workFunctor(_taskQueue->pop());

        }

    }
}

/**
 * 设置任务队列
 * @param taskQueue
 */
void DealThread::setTaskQueue(TaskQueuePtr taskQueue){
    _taskQueue=std::move(taskQueue);
}

/**
 * 注册任务回显
 * @param workFunctor
 */
void DealThread::setMessageCallBack(WorkFunctor workFunctor){
    _workFunctor=workFunctor;
}
