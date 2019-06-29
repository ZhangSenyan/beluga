//
// Created by zhsy on 19-6-8.
//

#include "Util.h"
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<cstdio>
#include <fcntl.h>

int socketCreate(int port){
    int fd=socket(AF_INET,SOCK_STREAM,0);
    if(fd==-1){
        perror("SOCKET Create");
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(port);
    server_addr.sin_addr.s_addr=htonl (INADDR_ANY);

    /*if (setnonblocking(fd) < 0) {
        perror("setnonblock error");
    }*/


    if(-1==bind(fd,(struct sockaddr*)&server_addr,sizeof(server_addr)))
        perror("SOCKET BIND");

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