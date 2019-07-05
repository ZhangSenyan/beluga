//
// Created by zhsy on 19-6-8.
//

#include "Server.h"
#include "base/Util.h"
#include <cstring>
#include <arpa/inet.h>
#include <iostream>
#include "net/Connection.h"


Server::Server(int port):_listenFd(socketCreate(port)),_running(false),_acceptThread(),_conns(){
    std::cout<<"Create Socket: Port="<<port<<std::endl;

}
Server::~Server(){

}
int Server::registerDealer(Dealer dealer){

}
void Server::startListen(){
    std::cout<<"Start listen"<<std::endl;
    _acceptThread.startLoop();
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
        std::shared_ptr<Connection> conn(new Connection(connFd,client_addr,&_acceptThread));
        _conns.insert(conn);
        conn->getChannel()->setEvents(EPOLLIN | EPOLLET);
        _acceptThread.addChannel(conn->getChannel());

    }
}
void Server::quit(){
    _running=false;
}