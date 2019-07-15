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

#include "beluga/base/Util.h"
#include "TCPClient.h"
int connectServer(const char* ip, int port){
    int fd=socket(AF_INET,SOCK_STREAM,0);
    if(fd==-1){
        perror("SOCKET Create");
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(port);
    server_addr.sin_addr.s_addr=inet_addr(ip);

    int ret=connect(fd,(const struct sockaddr*) &server_addr,sizeof(struct sockaddr));
    if(ret==-1){
        perror("connect:");
    }
    setnonblocking(fd);
    std::cout<<fd<<" ret="<<ret<<std::endl;
    return fd;
}
TCPClient::TCPClient(char * ip, int port):_fd(connectServer(ip,port)),_buffer(_fd){


}

TCPClient::~TCPClient(){

}
void TCPClient::setBlocking(){
    setblocking(_fd);
}
int TCPClient::getFD(){
    return _fd;
}
void TCPClient::write(std::string str){
    _buffer.write(str);
    _buffer.flushSend();
}
std::vector<std::string> TCPClient::readStream(){

    return _buffer.readStream();

}

