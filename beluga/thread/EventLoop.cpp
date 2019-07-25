/**
 * @author Zhang Senyan
 * Date: 2019-04-19
 *
 * EventLoop
 */

#include "EventLoop.h"
#include <cassert>

using beluga::CallFunc;

/**
 * 构造函数
 */
EventLoop::EventLoop(int size,int timer_ms):
            _epoll(size),
            _runing(false),
            _t(std::mem_fun(&EventLoop::HandleLoop),this),
            _timer(timer_ms),
            _threadId(_t.get_id()){

    //设置定时器，并添加进epoll事件中
    _timer.setHolder(this);
    _timer.getChannel()->setEvents(EPOLLIN | EPOLLET);

    _epoll.addChannel(_timer.getChannel());

}

EventLoop::~EventLoop(){

}

/**
 * 开始事件循环
 * @param method:
 *        1. detach ： 主线程与子线程分离
 *        2. join  ： 主线程等待子线程运行结束
 *
 */
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

/**
 * 事件循环主体
 */
void EventLoop::HandleLoop(){

    /**
     * 等待开始命令
     * 这里应该使用countdownlatch (待完善)
     */
    while(!_runing)usleep(10*1000);
    std::cout<<" start runing ... "<<std::endl;

    while(_runing){

        //等待事件到来
        Epoll::VectorCh channels=_epoll.poll();

        //处理事件
        for(auto channel:channels){
            //assert(!channel->expired());
            channel->handleEvents();
        }

        //处理其他事务
        doPendingFunctors();
    }
}

/**
 * 向 epoll 中添加 channel
 * @param channel
 */
void EventLoop::addChannel(Epoll::ptrChannel channel){
    _epoll.addChannel(channel);
}

void EventLoop::removeChannel(Epoll::ptrChannel channel){
    _epoll.removeChannel(channel);
}

/**
 * 处理注册的定时任务
 */
void EventLoop::timerHandle() {
    if(_timeFunctors.empty())
        return;
    
    for(auto timeFunctor:_timeFunctors){
        timeFunctor();
    }
}

/**
 * 添加定时任务
 * @param functor
 */
void EventLoop::addTimeFunctor(CallFunc functor){
    _timeFunctors.push_back(functor);
}


Epoll* EventLoop::getEpoll(){
    return &_epoll;
}

/**
 * 添加待处理事务
 * @param functor
 */
void EventLoop::addPendingFunctor(CallFunc functor){

    _pendingFunctors.push_back(functor);
}

/**
 * 待处理事务
 */
void EventLoop::doPendingFunctors(){

    std::vector<CallFunc> functors;

    //取出事务
    {
        std::lock_guard<std::mutex> l(_mutex);
        functors.swap(_pendingFunctors);
    }

    //调用事务注册的回调函数
    for(auto functor:functors){
        functor();
    }
}

