//
// Created by zhsy on 19-6-8.
//

#ifndef HCCSERVER_ACCEPTTHREAD_H
#define HCCSERVER_ACCEPTTHREAD_H


#include <iostream>
#include <pthread.h>

#include <unistd.h>
#include <thread>
#include <set>
#include "../net/Connection.h"
#include "../net/Epoll.h"
#include "../base/Timer.h"
//#include "ThreadData.h"

class AcceptThread {
public:
    AcceptThread(int size=10000,int timer_ms=50):_epoll(size),_runing(true),
    _t(std::mem_fun(&AcceptThread::HandleLoop),this),_timer(timer_ms){
        _timer.setHolder(this);
        _timer.getChannel()->setEvents(EPOLLIN | EPOLLET);
        _timer.setFlushBufferHandler(std::bind(&AcceptThread::flushBuffer,this));
        _epoll.addChannel(_timer.getChannel());
    }
    ~AcceptThread(){

    }
    void startLoop(){
        std::cout<<" start Loop ... "<<std::endl;
        _t.detach();
    }
    void HandleLoop(){
        std::cout<<" HandleLoop ... "<<std::endl;
        while(_runing){

            std::cout<<"Looping ... "<<std::endl;
            Epoll::VectorCh channels=_epoll.poll();
            for(auto channel:channels){
                channel->handleEvents();
            }
        }
    }
    void addChannel(Epoll::ptrChannel channel){
        _epoll.addChannel(channel);
    }
    void addConnction(std::shared_ptr<Connection> conn){
        connSet.insert(conn);
        _epoll.addChannel(conn->getChannel());
    }
    std::set<std::shared_ptr<Connection>>& getConnSet(){
        return connSet;
    }
    void flushBuffer(){
        std::cout<<"flushBuffer"<<std::endl;
        for(auto conn:connSet){
            conn->flushBuffer();
        }
    }
private:
    Epoll _epoll;
    Timer _timer;
    bool _runing;
    std::thread _t;
    std::set<std::shared_ptr<Connection>> connSet;
};


#endif //HCCSERVER_ACCEPTTHREAD_H
