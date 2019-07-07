//
// Created by zhsy on 19-7-6.
//

#include <iostream>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<stdio.h>
#include<arpa/inet.h>
#include <poll.h>
#include "base/Util.h"
#include "Client.h"
int connectServer(){
    int fd=socket(AF_INET,SOCK_STREAM,0);
    if(fd==-1){
        perror("SOCKET Create");
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(9000);
    server_addr.sin_addr.s_addr=inet_addr("10.20.4.5");

    int ret=connect(fd,(const struct sockaddr*) &server_addr,sizeof(struct sockaddr));
    if(ret==-1){
        perror("connect:");
    }
    setnonblocking(fd);
    std::cout<<fd<<" ret="<<ret<<std::endl;
    return fd;
}
Client::Client():_fd(connectServer()),_buffer(_fd){


}

Client::~Client(){

}
int Client::getFD(){
    return _fd;
}
void Client::write(std::string str){
    _buffer.write(str);
    _buffer.flushSend();
}
size_t Client::readAndDeal(){
    size_t count=0;
    auto recvV=_buffer.readStream();
    //"abcdefghigh"
    for(auto msg:recvV){
        if(msg=="hgihgfedcba")
            count++;
    }
    return count;
}

