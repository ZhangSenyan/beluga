//
// Created by zhsy on 19-6-29.
//


#include <functional>
#include <memory>
#include <cassert>

#include "Connection.h"
#include "beluga/thread/AcceptThread.h"
#include "beluga/task/CellTask.h"

Connection::Connection(int fd,struct sockaddr_in clientAddr):
        _fd(fd),_clientAddr(clientAddr),_acceptThread(),
        _expiredTime(20),_channel(new Channel(fd)),_buffer(fd,this){

    _channel->setReadHandler(std::bind(&Connection::handleRead,this));
    _channel->setWriteHandler(std::bind(&Connection::handleWrite,this));
    _channel->setErrorHandler(std::bind(&Connection::handleError,this));
    //_channel->setHolder(shared_from_this());
}

Connection::~Connection() {
    close(_fd);
    //std::cout<<"Drop connection : IP="<<getIP()<<" Port="<<getPort()<<std::endl;
    std::cout<<"Drop connection : "<<std::endl;
}

int Connection::getFd(){
    return _fd;
}

void Connection::setFd(int fd){
    _fd=fd;
}
std::string Connection::getIP(){
    return inet_ntoa(_clientAddr.sin_addr);
}
int Connection::getPort(){
    return ntohs(_clientAddr.sin_port);
}
void Connection::handleRead(){
    assert(!_acceptThread.expired());
    std::shared_ptr<AcceptThread> acceptThread=_acceptThread.lock();

    std::shared_ptr<TaskQueue> taskQueen=acceptThread->getTaskQueue();

    //更新时间轮-心跳检测
    acceptThread->updateConn(shared_from_this());

    std::vector<std::string> msgs=_buffer.readStream();
    // 基于当前系统的当前日期/时间
    //time_t now = time(0);
    for(auto msg:msgs){
    //    std::cout<<ctime(&now)<<"  "<<inet_ntoa(_clientAddr.sin_addr)<<"-"<<ntohs(_clientAddr.sin_port)
     //            <<":"<<msg<<std::endl;
        std::shared_ptr<CellTask> cellTaskptr(new CellTask(msg,shared_from_this()));
        taskQueen->push(cellTaskptr);
    }
    //std::string msg=_buffer.readSimple();
    //std::cout<<msg<<std::endl;
}

void Connection::handleWrite(){
    std::cout<<"handle write ... "<<std::endl;
    _buffer.flushSend();
    if(_buffer.empty()){
        closeListenEvent();
    }
}
void Connection::handleError() {
    std::cout<<"handle error ... "<<std::endl;
}
std::shared_ptr<Channel> Connection::getChannel(){
    return _channel;
}
int Connection::flushBuffer(){
    //std::cout<<"Connection::flushBuffer()"<<std::endl;
    return _buffer.flushSend();
}
std::shared_ptr<AcceptThread> Connection::getAcceptThread(){
    return _acceptThread.lock();
}
void Connection::openListenEvent(){
    _channel->addEvents(EPOLLOUT);
    getAcceptThread()->getEpoll()->updateChannel(_channel);
}
void Connection::closeListenEvent(){
    _channel->removeEvents(EPOLLOUT);
    getAcceptThread()->getEpoll()->updateChannel(_channel);
}
int Connection::writeBuffer(std::string result){

    _buffer.write(result);
}

void Connection::setAcceptThread(std::shared_ptr<AcceptThread> acceptThread){
    _acceptThread=acceptThread;
}