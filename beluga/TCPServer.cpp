//
// Created by zhsy on 19-6-8.
//

#include "TCPServer.h"
#include "beluga/base/Util.h"
#include <cstring>
#include <arpa/inet.h>
#include <iostream>
#include "beluga/net/Connection.h"
#include "beluga/task/TaskQueue.h"

using namespace beluga;
TCPServer::TCPServer(int port):_listenFd(socketCreate(port)),_running(false),_acceptThreads(4,150000),
        _dealThreads(8),_taskQueue(new TaskQueue()){
    std::cout<<"Create Socket: Port="<<port<<std::endl;
    _acceptThreads.setTaskQueue(_taskQueue);
    _dealThreads.setTaskQueue(_taskQueue);
    std::cout<<"Create TCPServer"<<std::endl;

}
TCPServer::~TCPServer(){

}

void TCPServer::startListen(){
    std::cout<<"Start listen"<<std::endl;
    _acceptThreads.startLoop();
    _dealThreads.startLoop();
    std::cout<<"has detach ..."<<std::endl;
    _running=true;
    struct sockaddr_in client_addr;
    socklen_t socklen=sizeof(struct sockaddr_in);
    while(_running){
        std::cout<<"listening ..."<<std::endl;
        memset(&client_addr,0,socklen);
        int connFd;
        connFd=accept(_listenFd,(struct sockaddr*)&client_addr,&socklen);
        std::cout << "Get a connect:IP=" << inet_ntoa(client_addr.sin_addr) <<
                " Port="<<ntohs(client_addr.sin_port)<<std::endl;
        if (setnonblocking(connFd) < 0) {
            perror("setnonblock error");
        }
        std::shared_ptr<Connection> newConn=std::make_shared<Connection>(connFd,client_addr);
        _onConnection(newConn);
        _acceptThreads.putConnection(std::move(newConn));

    }
}
void TCPServer::quit(){
    _running=false;
}
void TCPServer::setMessageCallBack(WorkFunctor onMessage){
    _dealThreads.setMessageCallBack(onMessage);
}
void TCPServer::setConnectionCallBack(ConnFunctor connFunctor){
    _onConnection=connFunctor;
}

void TCPServer::setDropConnectionCallBack(beluga::ConnFunctor connFunctor){
    _acceptThreads.setDropConnectionCallBack(connFunctor);
}