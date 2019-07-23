/**
 * @author Zhang Senyan
 * Date: 2019-06-12
 *
 */

#include <iostream>
#include <unistd.h>
#include <ctime>
#include <sys/time.h>

#include "beluga/base/Timer.h"
#include "beluga/net/Channel.h"
#include "beluga/base/Util.h"
#include "beluga/thread/EventLoop.h"
#include "beluga/log/LogStream.h"

//构造函数
Timer::Timer(int ms):
    _interTime(ms),
    _timefd(timerfd_init(ms)),
    _channel(new Channel(_timefd)),
    _eventLoop(nullptr),
    _timeHandler(nullptr){

    _channel->setReadHandler(std::bind(&Timer::handleRead,this));
    _channel->setWriteHandler(std::bind(&Timer::handleWrite,this));
    _channel->setErrorHandler(std::bind(&Timer::handleError,this));

}

//析构函数
Timer::~Timer(){

};

//获取定时器文件描述符
int Timer::getFD(){
    return _timefd;
}


void Timer::setHolder(EventLoop* eventLoop){
    _eventLoop=eventLoop;
}

/**
 * 处理定时任务
 * 1.在eventloop中注册的事件
 * 2.在timer中注册的事件
 *
 */
void Timer::handleRead(){

    //读取事件，否则下次不会触发定时
    uint64_t exp = 0;
    read(_timefd, &exp, sizeof(uint64_t));

    //处理所属线程的定时任务
    _eventLoop->timerHandle();

    //处理其他注册到定时器上定时任务
    if(_timeHandler)
        _timeHandler();

}


void Timer::handleWrite(){

}

//异常处理函数
void Timer::handleError(){
    LOG_ERR<<"Timer handle error!"<<LOG_ENDL;
}

//获取 timefd 对应的 channel
std::shared_ptr<Channel> Timer::getChannel(){
    return _channel;
}

//注册定时事件处理函数
void Timer::setTimeHandler(std::function<void(void)> timeHandler){
    _timeHandler=timeHandler;
}

/**
 * 获取系统当前的时间，主要用于日志系统
 * @return 以字符串形式返回
 *
 * 如果一秒中多次调用该函数，则只更新亚秒部分
 */
std::string Timer::getCurrentTime(){

    // 上次调用时间
    static struct timeval lasttv;

    // Sec 部分字符串
    static std::string SecString;

    //获取当前时间
    static struct timeval nowtv;
    gettimeofday(&nowtv,NULL);


    if(nowtv.tv_sec==lasttv.tv_sec){
        //SecString 不需要更新
        return SecString+std::to_string(nowtv.tv_usec);
    }else{
        lasttv.tv_sec=nowtv.tv_sec;

        //更新SecString
        SecString=ctime(&(nowtv.tv_sec));

        //添加格式字符
        SecString[SecString.length()-1]=':';
        return SecString+std::to_string(nowtv.tv_usec);
    }

}