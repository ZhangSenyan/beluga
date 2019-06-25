//
// Created by zhsy on 19-6-8.
//

#include "Server.h"
#include "util/Util.h"
#include<cstring>
#include<arpa/inet.h>
#include <iostream>
#include "net/Connection.h"

Server::Server(int port):_listenFd(socketStart(port)),_running(false){

}
Server::~Server(){

}
int Server::registerDealer(Dealer dealer){

}
void Server::startListen(){
    _running=true;
    struct sockaddr_in client_addr;
    socklen_t socklen=sizeof(struct sockaddr_in);
    while(_running){

        memset(&client_addr,0,socklen);
        int conn;
        conn=accept(_listenFd,(struct sockaddr*)&client_addr,&socklen);
        std::cout<<"Get a connect:"<<std::endl;
        _epoll.addConnection(new Connection(conn));


    }
}
void Server::quit(){
    _running=false;
}