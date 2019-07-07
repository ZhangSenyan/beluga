//
// Created by zhsy on 19-6-8.
//
#include <string>
#include "DealThread.h"

#include "CellTask.h"
#include "TaskQueue.h"

DealThread::DealThread():_runing(true),_t(std::mem_fun(&DealThread::HandleLoop),this){

}
DealThread::~DealThread(){

}
void DealThread::startLoop(){
    //std::cout<<" start Loop ... "<<std::endl;
    _t.detach();
}
void DealThread::HandleLoop(){
    //std::cout<<" HandleLoop ... "<<std::endl;
    while(_runing){
        //std::cout<<"Looping ... "<<std::endl;
        CellTaskPtr cellTask=_taskQueue->pop();
        std::string task=cellTask->getTask();
        std::string result(task.rbegin(),task.rend());
        //std::cout<<result<<std::endl;
        cellTask->respond(result);
    }
}
void DealThread::setTaskQueue(TaskQueuePtr taskQueue){
    _taskQueue=std::move(taskQueue);
}