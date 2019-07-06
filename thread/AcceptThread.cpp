//
// Created by zhsy on 19-6-8.
//

#include "AcceptThread.h"
#include "../net/Connection.h"

AcceptThread::AcceptThread(int size,int timer_ms):_epoll(size),_runing(true),
                                             _t(std::mem_fun(&AcceptThread::HandleLoop),this),_timer(timer_ms){
    _timer.setHolder(this);
    _timer.getChannel()->setEvents(EPOLLIN | EPOLLET);
    _timer.setFlushBufferHandler(std::bind(&AcceptThread::flushBuffer,this));
    _epoll.addChannel(_timer.getChannel());
}
AcceptThread::~AcceptThread(){

}
void AcceptThread::startLoop(){
    std::cout<<" start Loop ... "<<std::endl;
    _t.detach();
}
void AcceptThread::HandleLoop(){
    std::cout<<" HandleLoop ... "<<std::endl;
    while(_runing){

        std::cout<<"Looping ... "<<std::endl;
        Epoll::VectorCh channels=_epoll.poll();
        for(auto channel:channels){
            channel->handleEvents();
        }
    }
}
void AcceptThread::addChannel(Epoll::ptrChannel channel){
    _epoll.addChannel(channel);
}
void AcceptThread::addConnction(std::shared_ptr<Connection> conn){
    conn->getChannel()->setHolder(conn);
    conn->getChannel()->setEvents(EPOLLIN | EPOLLET);

    connSet.insert(conn);
    _epoll.addChannel(conn->getChannel());
}
std::set<std::shared_ptr<Connection>>& AcceptThread::getConnSet(){
    return connSet;
}
void AcceptThread::flushBuffer(){
    std::cout<<"flushBuffer"<<std::endl;
    for(auto conn:connSet){
        conn->flushBuffer();
    }
}
Epoll* AcceptThread::getEpoll(){
    return &_epoll;
}