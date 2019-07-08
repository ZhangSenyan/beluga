//
// Created by zhsy on 19-7-7.
//

#include "EventLoop.h"
EventLoop::EventLoop(int size,int timer_ms):_epoll(size),_runing(false),
_t(std::mem_fun(&EventLoop::HandleLoop),this),_timer(timer_ms){
    std::cout<<"EventLoop::EventLoop"<<std::endl;
    //usleep(10000);
    _timer.setHolder(this);
    _timer.getChannel()->setEvents(EPOLLIN | EPOLLET);
    //_timer.setTimeHandler(std::bind(&EventLoop::timerHandle,this));
    _epoll.addChannel(_timer.getChannel());
    std::cout<<"EventLoop::EventLoop end"<<std::endl;
}

EventLoop::~EventLoop(){

}

void EventLoop::startLoop(){
    _runing=true;
    std::cout<<" start Loop ... "<<std::endl;
    _t.detach();
}

void EventLoop::HandleLoop(){
    //等待开始命令
    while(!_runing)usleep(10*1000);
    std::cout<<" start runing ... "<<std::endl;
    while(_runing){
        Epoll::VectorCh channels=_epoll.poll();
        for(auto channel:channels){
            channel->handleEvents();
        }
    }
}

void EventLoop::addChannel(Epoll::ptrChannel channel){
    _epoll.addChannel(channel);
}







void EventLoop::timerHandle() {

}

Epoll* EventLoop::getEpoll(){
    return &_epoll;
}

