/**
 * @author Zhang Senyan
 * Date: 2019-05-15
 *
 * TCP Client
 */

#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <poll.h>

#include "beluga/base/Util.h"
#include "TCPClient.h"

/**
 * 网络函数-连接服务器
 * @param ip
 * @param port
 * @return   0 连接成功
 *          -1 连接失败
 */
int connectServer(const char* ip, int port){

    int fd=socket(AF_INET,SOCK_STREAM,0);
    if(fd==-1){
        perror("SOCKET Create");
    }

    //目标服务器地址
    struct sockaddr_in server_addr;
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(port);
    server_addr.sin_addr.s_addr=inet_addr(ip);

    int ret=connect(fd,(const struct sockaddr*) &server_addr,sizeof(struct sockaddr));
    if(ret==-1){
        perror("connect:");
    }

    //设置为非阻塞
    setnonblocking(fd);

    std::cout<<fd<<" ret="<<ret<<std::endl;
    return fd;
}

/**
 * 构造函数
 * @param ip
 * @param port
 */
TCPClient::TCPClient(const char * ip, int port):
            _fd(connectServer(ip,port)),
            _buffer(_fd){

}
TCPClient::~TCPClient(){

}


void TCPClient::setBlocking(){
    setblocking(_fd);
}

int TCPClient::getFD(){
    return _fd;
}

/**
 * 发送数据
 * @param str
 */
void TCPClient::write(std::string str){
    _buffer.write(str);
    _buffer.flushSend();
}

/**
 * 接收数据
 * @return
 */
std::vector<std::string> TCPClient::readStream(){

    return _buffer.readStream();
}

