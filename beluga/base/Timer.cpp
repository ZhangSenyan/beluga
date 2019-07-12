//
// Created by zhsy on 19-7-3.
//
#include <iostream>
#include <unistd.h>
#include <ctime>
#include <sys/time.h>

#include "Timer.h"
#include "beluga/net/Channel.h"
#include "Util.h"
#include "beluga/thread/EventLoop.h"

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

std::string Timer::getCurrentTime(){
    static struct timeval lasttv;
    static std::string SecString;
    static struct timeval nowtv;
    gettimeofday(&nowtv,NULL);
    time_t a;
    if(nowtv.tv_sec==lasttv.tv_sec){

        return SecString+std::to_string(nowtv.tv_usec);
    }else{

        lasttv.tv_sec=nowtv.tv_sec;
        SecString=ctime(&(nowtv.tv_sec));
        //去除尾部换行符号
        SecString[SecString.length()-1]=':';
        return SecString+std::to_string(nowtv.tv_usec);
    }

}