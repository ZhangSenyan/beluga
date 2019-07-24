/**
 * @author Zhang Senyan
 * Date: 2019-04-13
 *
 */

#include "Channel.h"
#include "beluga/net/Connection.h"
#include "beluga/thread/AcceptThread.h"
using namespace beluga;
Channel::Channel(int fd):_fd(fd),_events(0),_revents(0){

}
Channel::~Channel(){

}
void Channel::setHolder(const std::shared_ptr<Connection> &holder){
    _holder=holder;
}
void Channel::setReadHandler(CallFunc readhandler){
    _readhandler=readhandler;
}
void Channel::setWriteHandler(CallFunc writehandler){
    _writehandler=writehandler;
}
void Channel::setErrorHandler(CallFunc errorhandler){
    _errorhandler=errorhandler;
}
void Channel::handleEvents(){
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

    if ((_revents & EPOLLHUP) && !(_revents & EPOLLIN))
    {

        return;
    }
    if (_revents & EPOLLERR)
    {
        if (_errorhandler) _errorhandler();

        return;
    }
    if (_revents & (EPOLLIN | EPOLLPRI | EPOLLRDHUP))
    {
        if(_readhandler)_readhandler();
    }
    if (_revents & EPOLLOUT)
    {
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
void Channel::setEvents(__uint32_t events){
    _events=events;
}
__uint32_t Channel::getRevents(){
    return  _revents;
}
void Channel::setRevents(__uint32_t revents){
    _revents=revents;
}
void Channel::addEvents(__uint32_t events){
    _events=_events|events;
}
void Channel::removeEvents(__uint32_t events){
    _events=_events & (~events);
}
void Channel::updateEvents(){
    /*
    std::shared_ptr<Connection> holder=_holder.lock();
    holder->getAcceptThread()->getEpoll()->updateChannel(shared_from_this());*/
}
bool Channel::expired(){
    return _holder.expired();
}