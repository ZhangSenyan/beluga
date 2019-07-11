//
// Created by zhsy on 19-7-3.
//
#include <iostream>
#include <unistd.h>
#include "Timer.h"
#include "Channel.h"
#include "Util.h"
#include "EventLoop.h"

Timer::Timer(int ms):interTime(ms),timefd(timerfd_init(ms)),
_channel(new Channel(timefd)),_eventLoop(nullptr),_timeHandler(nullptr){
    _channel->setReadHandler(std::bind(&Timer::handleRead,this));
    _channel->setWriteHandler(std::bind(&Timer::handleWrite,this));
    _channel->setErrorHandler(std::bind(&Timer::handleError,this));
}
Timer::~Timer(){

};

int Timer::getFD(){
    return timefd;
}
void Timer::setHolder(EventLoop* eventLoop){
    _eventLoop=eventLoop;
}
void Timer::handleRead(){
    //std::cout<<"timer handle read"<<std::endl;
    uint64_t exp = 0;
    read(timefd, &exp, sizeof(uint64_t));
    _eventLoop->timerHandle();
    if(_timeHandler)
        _timeHandler();
}
void Timer::handleWrite(){
    std::cout<<"timer handle write"<<std::endl;
}
void Timer::handleError(){
    std::cout<<"timer handle Error"<<std::endl;
}
std::shared_ptr<Channel> Timer::getChannel(){
    return _channel;
}
void Timer::setTimeHandler(std::function<void(void)> timeHandler){
    _timeHandler=timeHandler;
}