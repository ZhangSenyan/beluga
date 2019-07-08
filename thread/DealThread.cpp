//
// Created by zhsy on 19-6-8.
//
#include <string>
#include "DealThread.h"

#include "CellTask.h"
#include "TaskQueue.h"
#include "unistd.h"
DealThread::DealThread():_runing(false),_t(std::mem_fun(&DealThread::HandleLoop),this),_taskQueue(nullptr){
    std::cout<<"DealThread::DealThread"<<std::endl;
}
DealThread::~DealThread(){

}
void DealThread::startLoop(){
    //std::cout<<" start Loop ... "<<std::endl;
    _runing=true;
    _t.detach();
}
void DealThread::HandleLoop(){
    //std::cout<<" HandleLoop ... "<<std::endl;
    while(!_runing) usleep(10*1000);
    //这里必须要设置等待否则在其他对象初始化之前调用对象
    while(_runing){
        //std::cout<<"Looping ... "<<std::endl;
        if(_taskQueue.get()){
            CellTaskPtr cellTask=_taskQueue->pop();
            std::string task=cellTask->getTask();
            std::string result(task.rbegin(),task.rend());
            //std::cout<<result<<std::endl;
            cellTask->respond(result);
        }

    }
}


void DealThread::setTaskQueue(TaskQueuePtr taskQueue){
    _taskQueue=std::move(taskQueue);
}
