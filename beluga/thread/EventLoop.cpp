//
// Created by zhsy on 19-7-7.
//

#include "EventLoop.h"
#include <cassert>
EventLoop::EventLoop(int size,int timer_ms):_epoll(size),_runing(false),
_t(std::mem_fun(&EventLoop::HandleLoop),this),_timer(timer_ms),_threadId(_t.get_id()){

    //usleep(10000);
    _timer.setHolder(this);
    _timer.getChannel()->setEvents(EPOLLIN | EPOLLET);
    //_timer.setTimeHandler(std::bind(&EventLoop::timerHandle,this));
    _epoll.addChannel(_timer.getChannel());

}

EventLoop::~EventLoop(){

}

void EventLoop::startLoop(beluga::LoopMethod method){
    _runing=true;
    std::cout<<" start Loop ... "<<std::endl;
    switch(method){
        case beluga::LoopMethod::detach:{
            _t.detach();
            break;
        }
        case beluga::LoopMethod::join:{
            _t.join();
            break;
        } 
        default:{
            _t.detach();
            break;
        }
    }
    
}

void EventLoop::HandleLoop(){
    //等待开始命令
    while(!_runing)usleep(10*1000);
    std::cout<<" start runing ... "<<std::endl;
    while(_runing){
        Epoll::VectorCh channels=_epoll.poll();
        for(auto channel:channels){
            //assert(!channel->expired());
            channel->handleEvents();
        }
        doPendingFunctors();
    }
}

void EventLoop::addChannel(Epoll::ptrChannel channel){
    _epoll.addChannel(channel);
}
void EventLoop::removeChannel(Epoll::ptrChannel channel){
    _epoll.removeChannel(channel);
}

void EventLoop::timerHandle() {
    if(_timeFunctors.empty())
        return;
    
    for(auto timeFunctor:_timeFunctors){
        timeFunctor();
    }
}

void EventLoop::addTimeFunctor(Functor functor){
    _timeFunctors.push_back(functor);
}

Epoll* EventLoop::getEpoll(){
    return &_epoll;
}

void EventLoop::addPendingFunctor(Functor functor){

    _pendingFunctors.push_back(functor);
}
void EventLoop::doPendingFunctors(){

    std::vector<Functor> functors;
    {
        std::lock_guard<std::mutex> l(_mutex);
        functors.swap(_pendingFunctors);
    }

    for(auto functor:functors){
        functor();
    }
}

