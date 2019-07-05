//
// Created by zhsy on 19-6-16.
//

#ifndef HCCSERVER_CHANNEL_H
#define HCCSERVER_CHANNEL_H


#include <functional>
#include <sys/epoll.h>
#include <memory>
class Connection;

class Channel {

public:
    typedef std::function<void(void)> CallFunc;
    Channel(int fd):_fd(fd){

    }
    ~Channel(){

    }
private:
    CallFunc _readhandler;
    CallFunc _writehandler;
    CallFunc _errorhandler;
    int _fd;
    __uint32_t _events;
    __uint32_t _revents;
    std::weak_ptr<Connection> _holder;

public:

    void setHolder(std::shared_ptr<Connection> holder){
        _holder=holder;
    }
    void setReadHandler(CallFunc readhandler){
        _readhandler=readhandler;
    }
    void setWriteHandler(CallFunc writehandler){
        _writehandler=writehandler;
    }
    void setErrorHandler(CallFunc errorhandler){
        _errorhandler=errorhandler;
    }
    void handleEvents(){
        /*
         * EPOLLIN ：表示对应的文件描述符可以读（包括对端SOCKET正常关闭）；
         * EPOLLOUT：表示对应的文件描述符可以写；
         * EPOLLPRI：表示对应的文件描述符有紧急的数据可读（这里应该表示有带外数据到来）；
         * EPOLLERR：表示对应的文件描述符发生错误；
         * EPOLLHUP：表示对应的文件描述符被挂断；
         * EPOLLET： 将EPOLL设为边缘触发(Edge Triggered)模式，这是相对于水平触发(Level Triggered)来说的。
         * EPOLLONESHOT：只监听一次事件，当监听完这次事件之后，如果还需要继续监听这个socket的话，
         * 需要再次把这个socket加入到EPOLL队列里
         */

        _events = 0;

        if ((_revents & EPOLLHUP) && !(_revents & EPOLLIN))
        {
            _events = 0;
            return;
        }
        if (_revents & EPOLLERR)
        {
            if (_errorhandler) _errorhandler();
            _events = 0;
            return;
        }
        if (_revents & (EPOLLIN | EPOLLPRI | EPOLLRDHUP))
        {
            _readhandler();
        }
        if (_revents & EPOLLOUT)
        {
            _writehandler();
        }

    }
    int getFd(){
        return _fd;
    }
    __uint32_t getEvents(){
        return  _events;
    }
    void setEvents(__uint32_t events){
        _events=events;
    }
    __uint32_t getRevents(){
        return  _revents;
    }
    void setRevents(__uint32_t revents){
        _revents=revents;
    }


};


#endif //HCCSERVER_CHANNEL_H




