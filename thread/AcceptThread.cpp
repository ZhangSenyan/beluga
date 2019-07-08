//
// Created by zhsy on 19-6-8.
//

#include "AcceptThread.h"
#include "Connection.h"

AcceptThread::AcceptThread(int size,int timer_ms):EventLoop(size,timer_ms){
    std::cout<<"AcceptThread::AcceptThread"<<std::endl;
}

AcceptThread::~AcceptThread(){

}
void AcceptThread::addConnction(std::shared_ptr<Connection> conn){
    conn->getChannel()->setHolder(conn);
    conn->getChannel()->setEvents(EPOLLIN | EPOLLET);
    connSet.insert(conn);
    addChannel(conn->getChannel());
}

std::set<std::shared_ptr<Connection>>& AcceptThread::getConnSet(){
    return connSet;
}

void AcceptThread::timerHandle() {
    for(auto conn:connSet){
        conn->flushBuffer();
    }
}

void AcceptThread::setTaskQueue(std::shared_ptr<TaskQueue> taskQueue){
    _taskQueue=taskQueue;
}
std::shared_ptr<TaskQueue> AcceptThread::getTaskQueue(){
    return _taskQueue;
}

int AcceptThread::getConnSize(){
    return connSet.size();
}

