/**
 * @author Zhang Senyan
 * Date: 2019-05-05
 *
 */

#include "TaskQueue.h"
#include <iostream>

/**
 * 构造函数
 */
TaskQueue::TaskQueue(size_t maxSize):_maxSize(maxSize){

}
TaskQueue::~TaskQueue(){

}

/**
 * 添加任务
 * @param cellTaskPtr
 * @return false : 任务队列超过限定值，添加失败
 *
 */
bool TaskQueue::push(CellTaskPtr cellTaskPtr){
    {
        std::unique_lock<std::mutex> l(_mutex);

        //检测容量是否超过限定值，如果超过则丢弃
        if (_taskQueue.size() > _maxSize)
            return false;

        _taskQueue.push(cellTaskPtr);
    }

    _cv.notify_all();
    return true;
}

/**
 * 计算线程取出任务
 * @return CellTask
 */
TaskQueue::CellTaskPtr TaskQueue::pop(){
    std::unique_lock<std::mutex> l(_mutex);
    /**
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

/**
 * 取出队列首部任务
 * @return
 */
TaskQueue::CellTaskPtr TaskQueue::front(){
    std::lock_guard<std::mutex> l(_mutex);
    return _taskQueue.front();
}

/**
 * 清空队列
 */
void TaskQueue::clear(){
    std::lock_guard<std::mutex> l(_mutex);
    while(!_taskQueue.empty()){
        _taskQueue.pop();
    }
}

/**
 * @return 当前任务队列大小
 */
size_t TaskQueue::size(){
    std::lock_guard<std::mutex> l(_mutex);
    return _taskQueue.size();
}

/**
 * 重置任务队列的限定值
 * @param size
 */
void TaskQueue::reverse(size_t size){
    std::lock_guard<std::mutex> l(_mutex);
    _maxSize=size;
}

size_t TaskQueue::capacity(){
    std::lock_guard<std::mutex> l(_mutex);
    return _maxSize;
}

