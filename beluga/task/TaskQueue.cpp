//
// Created by zhsy on 19-6-8.
//

#include "TaskQueue.h"
#include <iostream>
TaskQueue::TaskQueue(size_t maxSize):_maxSize(maxSize){
    std::cout<<"TaskQueue::TaskQueue"<<std::endl;
}
TaskQueue::~TaskQueue(){

}
bool TaskQueue::push(CellTaskPtr cellTaskPtr){
    {
        std::unique_lock<std::mutex> l(_mutex);
        if (_taskQueue.size() > _maxSize)
            return false;
        _taskQueue.push(cellTaskPtr);
    }
    _cv.notify_all();
    return true;
}

TaskQueue::CellTaskPtr TaskQueue::pop(){
    std::unique_lock<std::mutex> l(_mutex);
    /*
     * 因为要与条件变量连用，所以使用std::unique_lock
     * std::lock_guard 没有lock和unlock函数，不符合要求
     */
    while(_taskQueue.empty()){
        //预防虚假唤醒
        _cv.wait(l);
    }
    CellTaskPtr temp=_taskQueue.front();
    _taskQueue.pop();
    return temp;

}
TaskQueue::CellTaskPtr TaskQueue::front(){
    std::lock_guard<std::mutex> l(_mutex);
    return _taskQueue.front();
}
void TaskQueue::clear(){
    std::lock_guard<std::mutex> l(_mutex);
    while(!_taskQueue.empty()){
        _taskQueue.pop();
    }
}
size_t TaskQueue::size(){
    std::lock_guard<std::mutex> l(_mutex);
    return _taskQueue.size();
}
void TaskQueue::reverse(size_t size){
    std::lock_guard<std::mutex> l(_mutex);
    _maxSize=size;
}
size_t TaskQueue::capacity(){
    std::lock_guard<std::mutex> l(_mutex);
    return _maxSize;
}