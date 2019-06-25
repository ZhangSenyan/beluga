//
// Created by zhsy on 19-6-15.
//


#include <iostream>
#include<cstdio>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<cstring>
#include<arpa/inet.h>
#include <sys/epoll.h>
//#include <bits/fcntl-linux.h>
#include <fcntl.h>


#define MAXEPOLLSIZE 10000

int setnonblocking(int sockfd)
{
    if (fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFD, 0)|O_NONBLOCK) == -1) {
        return -1;
    }
    return 0;
}

using namespace std;
int main() {

    // 创建 Socket
    int listenfd=socket(AF_INET,SOCK_STREAM,0);
    if(listenfd==-1){
        perror("SOCKET Create");
    }

    //地址复用
    int opt = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    //设置为非阻塞
    if (setnonblocking(listenfd) < 0) {
        perror("setnonblock error");
    }

    //绑定接口
    struct sockaddr_in server_addr, client_addr;
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(10000);
    server_addr.sin_addr.s_addr=htonl (INADDR_ANY);
    if(-1==bind(listenfd,(struct sockaddr*)&server_addr,sizeof(server_addr)))
        perror("SOCKET BIND");

    //注册监听
    if(-1==listen(listenfd,SOMAXCONN))
        perror("SOCKET LISTEN");

    /* 创建 epoll 句柄，把监听 socket 加入到 epoll 集合里 */
    int kdpfd = epoll_create(MAXEPOLLSIZE);
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = listenfd;
    if (epoll_ctl(kdpfd, EPOLL_CTL_ADD, listenfd, &ev) < 0)
    {
        fprintf(stderr, "epoll set insertion error: fd=%d\n", listenfd);
        return -1;
    }

    int curfds = 1;
    struct epoll_event events[MAXEPOLLSIZE];

    for(;;){

        int nfds = epoll_wait(kdpfd, events, curfds, -1);
        cout<<"recv nfds="<<nfds<<endl;
        if (nfds == -1)
        {
            perror("epoll_wait");
            continue;
        }

        for(int i=0;i<nfds;i++){
            if(events[i].data.fd==listenfd){

                socklen_t socklen=sizeof(struct sockaddr_in);
                memset(&client_addr,0,sizeof(struct sockaddr_in));
                int connfd=accept(listenfd,(struct sockaddr*)&client_addr,&socklen);
                if (-1==connfd)
                {
                    fprintf(stderr, "accept error");
                    continue;
                }
                //设置为非阻塞
                if (setnonblocking(connfd) < 0) {
                    perror("setnonblock error");
                }

                if(curfds>=MAXEPOLLSIZE){
                    close(connfd);
                    continue;
                }


                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = connfd;
                if (epoll_ctl(kdpfd, EPOLL_CTL_ADD, connfd, &ev) < 0)
                {
                    fprintf(stderr, "epoll set insertion error: fd=%d\n", listenfd);
                    return -1;
                }
                curfds++;
            }else{
                //接收数据
                int readfd=events[i].data.fd;
                char recvbuff[100]={0};
                ssize_t recvlen=read(readfd,recvbuff,sizeof(recvbuff));
                if (recvlen == 0) {
                    printf("client close the connection\n");
                    close(readfd);
                    nfds--;
                    epoll_ctl(kdpfd, EPOLL_CTL_DEL, readfd,NULL);

                    return -1;
                }
                if (recvlen < 0) {
                    perror("read error");
                    close(readfd);
                    nfds--;
                    epoll_ctl(kdpfd, EPOLL_CTL_DEL, readfd,NULL);
                    return -1;
                }

                //没有任务,执行 echo
                write(readfd,recvbuff,recvlen);
                printf("%s\n",recvbuff);
                printf("5\n");

            }
        }

    }



    socklen_t socklen=sizeof(struct sockaddr_in);

    memset(&client_addr,0,sizeof(struct sockaddr_in));
    int conn=accept(listenfd,(struct sockaddr*)&client_addr,&socklen);
    char recvbuff[100]={0};

    while(1){
        ssize_t recvlen=read(conn,recvbuff,sizeof(recvbuff));
        write(conn,recvbuff,recvlen);
        printf("%s\n",recvbuff);
        printf("5\n");
    }


    return 0;
}