//
// Created by zhsy on 19-7-3.
//

#ifndef HCCSERVER_TIMER_H
#define HCCSERVER_TIMER_H

#include <memory>
#include <iostream>
#include <unistd.h>
#include "Util.h"
#include "../net/Channel.h"

class AcceptThread;
class Timer :std::enable_shared_from_this<Timer>{
public:
    Timer(int ms):interTime(ms),timefd(timerfd_init(ms)),_channel(new Channel(timefd)){
        _channel->setReadHandler(std::bind(&Timer::handleRead,this));
        _channel->setWriteHandler(std::bind(&Timer::handleWrite,this));
        _channel->setErrorHandler(std::bind(&Timer::handleError,this));

    }
    ~Timer(){

    };

    int getFD(){
        return timefd;
    }
    void setHolder(AcceptThread* acceptThread){
        _acceptThread=acceptThread;
    }
    void handleRead(){
        //std::cout<<"timer handle read"<<std::endl;
        uint64_t exp = 0;
        read(timefd, &exp, sizeof(uint64_t));
        _flushBufferHandler();
    }
    void handleWrite(){
        std::cout<<"timer handle write"<<std::endl;
    }
    void handleError(){
        std::cout<<"timer handle Error"<<std::endl;
    }
    std::shared_ptr<Channel> getChannel(){
        return _channel;
    }
    void setFlushBufferHandler(std::function<void(void)> flushBufferHandler){
        _flushBufferHandler=flushBufferHandler;
    }
private:
    int timefd;
    int interTime;
    std::shared_ptr<Channel> _channel;
    AcceptThread* _acceptThread;
    std::function<void(void)> _flushBufferHandler;
};


#endif //HCCSERVER_TIMER_H
