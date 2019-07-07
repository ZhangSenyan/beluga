//
// Created by zhsy on 19-6-8.
//
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <sys/timerfd.h>

#include "Util.h"

int socketCreate(int port){
    //创建SOCKET
    int fd=socket(AF_INET,SOCK_STREAM,0);
    if(fd==-1){
        perror("SOCKET Create");
    }

    //设定多路地址复用
    int optval = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
    {
        perror("setsockopt bind\n");
        exit(0);
    }

    //设置非阻塞
    /*if (setnonblocking(fd) < 0) {
        perror("setnonblock error");
    }*/


    //绑定地址
    struct sockaddr_in server_addr;
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(port);
    server_addr.sin_addr.s_addr=htonl (INADDR_ANY);

    if(-1==bind(fd,(struct sockaddr*)&server_addr,sizeof(server_addr)))
        perror("SOCKET BIND");

    //启动监听
    if(-1==listen(fd,SOMAXCONN))
        perror("SOCKET LISTEN");
    return fd;
}

int setnonblocking(int sockfd)
{
    if (fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFD, 0)|O_NONBLOCK) == -1) {
        return -1;
    }
    return 0;
}

ssize_t readn(int fd,char *ptr,size_t n){

    size_t nleft=n;
    ssize_t nread;

    nleft=n;
    while(nleft>0){
        if((nread=read(fd,ptr,nleft))<0){
            if(nleft==n)
                return -1;
            else
                break;
        }else if(nread==0)
            break;
        nleft-=nread;
        ptr+=nread;
    }
    return (n-nleft);

}

ssize_t writen(int fd,char *ptr,size_t n){
    size_t nleft=n;
    ssize_t nwrite;

    while(nleft>0){
        if((nwrite=write(fd,ptr,nleft))<0){
            if(nleft==n)
                return -1;
            else
                break;
        }else if(nwrite==0)
            break;
        nleft-=nwrite;
        ptr+=nwrite;
    }
    return (n-nleft);

}


int timerfd_init(unsigned long ms)
{
    int tmfd;
    int ret;
    struct itimerspec new_value;
    new_value.it_value.tv_sec = ms/1000;
    new_value.it_value.tv_nsec =(ms%1000)*1000000;
    new_value.it_interval.tv_sec = ms/1000;
    new_value.it_interval.tv_nsec = (ms%1000)*1000000;

    tmfd = timerfd_create(CLOCK_MONOTONIC, 0);
    if (tmfd < 0) {
        std::cout<<"timerfd_create error, Error:["<<errno<<"]"<<std::endl;
        return -1;
    }
    ret = timerfd_settime(tmfd, 0, &new_value, NULL);
    if (ret < 0) {
        std::cout<<"timerfd_settime error, Error:["<<errno<<"]"<<std::endl;
        close(tmfd);
        return -1;
    }
    return tmfd;
}