/**
 * @author Zhang Senyan
 * Date: 2019-04-13
 *
 */

#include "Channel.h"
#include "beluga/net/Connection.h"
#include "beluga/thread/AcceptThread.h"

using namespace beluga;

//构造函数
Channel::Channel(int fd):_fd(fd),_events(0),_revents(0){

}
Channel::~Channel(){

}

/**
 * 设定所关联Connection
 * @param holder -> Connection
 */
void Channel::setHolder(const std::shared_ptr<Connection> &holder){
    _holder=holder;
}

/**
 * 注册读事件
 * @param readhandler
 */
void Channel::setReadHandler(CallFunc readhandler){
    _readhandler=readhandler;
}

/**
 * 注册写事件
 * @param writehandler
 */
void Channel::setWriteHandler(CallFunc writehandler){
    _writehandler=writehandler;
}

/**
 * 注册错误事件
 * @param errorhandler
 */
void Channel::setErrorHandler(CallFunc errorhandler){
    _errorhandler=errorhandler;
}

/**
 * 事件处理函数
 *
 */
void Channel::handleEvents(){


    if ((_revents & EPOLLHUP) && !(_revents & EPOLLIN))
    {
        /**
         * 表示对应的文件描述符被挂断
         * 且对应的文件不可以读
         */
        return;
    }
    if (_revents & EPOLLERR)
    {
        //对应的文件描述符发生错误
        if (_errorhandler) _errorhandler();
        return;
    }
    if (_revents & (EPOLLIN | EPOLLPRI | EPOLLRDHUP))
    {
        /**
         * 1.文件描述符可以读
         * 2.有紧急的数据可读
         * 3.对应的文件描述符被挂断
         */
        if(_readhandler)_readhandler();
    }
    if (_revents & EPOLLOUT)
    {
        //对应的文件描述符可以写
        if(_writehandler)_writehandler();
    }

}


int Channel::getFD(){
    return _fd;
}

void Channel::setFD(int fd){
    _fd=fd;
}

__uint32_t Channel::getEvents(){
    return  _events;
}
/**
 * 设定事件，注册channel时调用
 * @param events
 */
void Channel::setEvents(__uint32_t events){
    _events=events;
}

__uint32_t Channel::getRevents(){
    return  _revents;
}

/**
 * 设定返回事件，注册channel时调用
 * @param events
 */
void Channel::setRevents(__uint32_t revents){
    _revents=revents;
}

/**
 * 添加事件
 * @param events
 */
void Channel::addEvents(__uint32_t events){
    _events=_events|events;
}

/**
 * 删除事件
 * @param events
 */
void Channel::removeEvents(__uint32_t events){
    _events=_events & (~events);
}


bool Channel::expired(){
    return _holder.expired();
}