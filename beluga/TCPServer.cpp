/**
 * @author Zhang Senyan
 * Date: 2019-05-09
 *
 * TCP Server
 */


#include <cstring>
#include <arpa/inet.h>
#include <iostream>

#include "TCPServer.h"
#include "beluga/base/Util.h"
#include "beluga/net/Connection.h"
#include "beluga/task/TaskQueue.h"


using namespace beluga;

TCPServer::TCPServer(int port):
    _listenFd(socketCreate(port)),
    _running(false),
    _acceptThreads(4,150000),
    _dealThreads(4),
    _taskQueue(new TaskQueue()),
    _eventLoop(50000,1000),
    _listenChannel(new Channel(0)){

    //设置任务队列
    _acceptThreads.setTaskQueue(_taskQueue);
    _dealThreads.setTaskQueue(_taskQueue);

    //配置监听Channel，添加到epoll
    _listenChannel->setFD(_listenFd);
    _listenChannel->setReadHandler(std::bind(&TCPServer::onAccept,this));
    _listenChannel->setEvents(EPOLLIN| EPOLLET| EPOLLERR);
    _eventLoop.addChannel(_listenChannel);

    std::cout<<"Create TCPServer Success"<<std::endl;

}
TCPServer::~TCPServer(){

}

/**
 * 添加定时任务
 * @param func
 */
void TCPServer::setTimerCallBack(CallFunc func){
    _eventLoop.addTimeFunctor(func);
}

void TCPServer::startListen(){

    //std::cout<<"Start listen"<<std::endl;

    //启动 IO线程 和 计算线程
    _acceptThreads.startLoop();
    _dealThreads.startLoop();

    //std::cout<<"has detach ..."<<std::endl;

    _running=true;
    _eventLoop.startLoop(beluga::LoopMethod::join);

}

/**
 * 监听到新连接
 */
void TCPServer::onAccept(){


    struct sockaddr_in client_addr;
    socklen_t socklen=sizeof(struct sockaddr_in);
    memset(&client_addr,0,socklen);

    int connFd;
    connFd=accept(_listenFd,(struct sockaddr*)&client_addr,&socklen);

    std::cout << "Get a connect:IP=" << inet_ntoa(client_addr.sin_addr) <<
              " Port="<<ntohs(client_addr.sin_port)<<std::endl;

    //将该连接设为非阻塞IO
    if (setnonblocking(connFd) < 0) {
        perror("setnonblock error");
    }

    std::shared_ptr<Connection> newConn=std::make_shared<Connection>(connFd,client_addr);

    //用户注册的回显
    _onConnection(newConn);

    //加入IO线程池
    _acceptThreads.putConnection(std::move(newConn));
}

/**
 * 关闭TCPServer
 */
void TCPServer::quit(){
    _running=false;
    //其他操作，待完善
}

/**
 * 设置用户回调
 * @param onMessage
 */
void TCPServer::setMessageCallBack(WorkFunctor onMessage){
    _dealThreads.setMessageCallBack(onMessage);
}
void TCPServer::setConnectionCallBack(ConnFunctor connFunctor){
    _onConnection=connFunctor;
}
void TCPServer::setDropConnectionCallBack(beluga::ConnFunctor connFunctor){
    _acceptThreads.setDropConnectionCallBack(connFunctor);
}