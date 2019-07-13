//
// Created by zhsy on 19-6-8.
//

#include "AcceptThread.h"


//#include "net/Connection.h"
#include <cassert>
AcceptThread::AcceptThread(int size,int timer_ms):EventLoop(size,timer_ms),_timingWheel(10),_timeCount(0){
    std::cout<<"AcceptThread::AcceptThread"<<std::endl;
}

AcceptThread::~AcceptThread(){

}
void AcceptThread::addConnction(std::shared_ptr<Connection> conn){
    conn->getChannel()->setHolder(conn);
    conn->getChannel()->setEvents(EPOLLIN | EPOLLET);
    assert(connMap.find(conn->getFd())==connMap.end());
    connMap[conn->getFd()]=conn;
    addChannel(conn->getChannel());
    _timingWheel.addConn(conn->getFd());
}
void AcceptThread::removeConnction(std::shared_ptr<Connection> conn){
    /*
     *仅仅是关闭对应的消息接收，具体文件描述符关闭要等到全部消息队列中该连接消息清空
     *时间轮中无需清理，心跳自检程序会自动清除
     */
    _onDropConntion(conn);
    removeChannel(conn->getChannel());
    connMap.erase(conn->getFd());

}
void AcceptThread::removeConnction(int connfd){
    /*
     *仅仅是关闭对应的消息接收，具体文件描述符关闭要等到全部消息队列中该连接消息清空
     *时间轮中无需清理，心跳自检程序会自动清除
     *
     * 注意：removeChannel应该在erase前
     */
    removeChannel(connMap[connfd]->getChannel());
    connMap.erase(connfd);

}
std::map<int,std::shared_ptr<Connection>>& AcceptThread::getConnMap(){
    return connMap;
}
void AcceptThread::timerHandle() {
    //50ms 计时任务

    _timeCount+=50;
    timerHandle50();
    if(_timeCount>=1000){
        _timeCount=0;
        timerHandle1000();
    }
}
void AcceptThread::timerHandle1000() {

    //1s 计时任务
    std::unordered_set<int> dlconns=_timingWheel.step();
    addPendingFunctor([dlconns,this](){
        for(auto &dlconnfd:dlconns){
            removeConnction(dlconnfd);
        }
    });

}
void AcceptThread::timerHandle50() {
    //50ms 计时任务
    for(auto &connPair:connMap){
        connPair.second->flushBuffer();
    }
}

void AcceptThread::setTaskQueue(const std::shared_ptr<TaskQueue>& taskQueue){
    _taskQueue=taskQueue;
}
std::shared_ptr<TaskQueue> AcceptThread::getTaskQueue(){
    return _taskQueue;
}

int AcceptThread::getConnSize(){
    return connMap.size();
}
int AcceptThread::updateConn(Connection::ConnPtr conn){
    _timingWheel.updateConn(conn->getFd());
}

void AcceptThread::setDropConnectionCallBack(beluga::ConnFunctor dropFunctor){
    _onDropConntion=dropFunctor;
}