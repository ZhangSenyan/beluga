/**
 * @author Zhang Senyan
 * Date: 2019-04-03
 *
 * 常用工具
 */

#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <sys/timerfd.h>

#include "beluga/base/Util.h"

/**
 * 创建socket 并启动监听
 * @param port ： 监听端口
 * @return 监听句柄
 */
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

/**
 * 将文件句柄设为非阻塞
 * @param sockfd ：文件句柄
 * @return  0 : 成功
 *         -1： 失败
 */
int setnonblocking(int sockfd)
{
    if (fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFD, 0)|O_NONBLOCK) == -1) {
        return -1;
    }
    return 0;
}

/**
 * 将文件句柄设为阻塞
 * @param sockfd ：文件句柄
 * @return  0 : 成功
 *         -1： 失败
 */
int setblocking(int sockfd)
{
    if (fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFD, 0)&(~O_NONBLOCK)) == -1) {
        return -1;
    }
    return 0;
}

/**
 * 从当前文件句柄中读取n个字节，直到读满或者错误
 *
 * @param   fd   文件句柄
 * @param   ptr  输入缓冲区
 * @param   n    要读取的字节数量
 *
 * @return  -1   读取0字节，出现错误
 *          >0   实际读取的字节数量
 */
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

/**
 * 向当前文件句柄中写入n个字节
 *
 * @param   fd   文件句柄
 * @param   ptr  输出缓冲区
 * @param   n    要写入的字节数量
 *
 * @return  -1   写入0字节，出现错误
 *          >0   实际写入的字节数量
 */
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

/**
 * 创建定时器句柄
 * @param ms 定时时间
 * @return -1 创建失败
 *         >0 定时器句柄
 */
int timerfd_init(unsigned long ms)
{

    struct itimerspec new_value;
    new_value.it_value.tv_sec = ms/1000;
    new_value.it_value.tv_nsec =(ms%1000)*1000000;
    new_value.it_interval.tv_sec = ms/1000;
    new_value.it_interval.tv_nsec = (ms%1000)*1000000;

    //创建定时器句柄
    int tmfd = timerfd_create(CLOCK_MONOTONIC, 0);
    if (tmfd < 0) {
        perror("TimerFd Create");
        return -1;
    }

    //设置定时时间
    if (0 > timerfd_settime(tmfd, 0, &new_value, NULL)) {
        perror("Set Timer");
        close(tmfd);
        return -1;
    }

    return tmfd;
}


/**
 * 获取当前工作目录
 * @return 以字符串形式返回
 */
std::string getCWD(){
    std::string cwd;
    char *buffer;
    if((buffer = getcwd(NULL, 0)) == NULL)
    {
        perror("getcwd error");
    }
    else
    {
        cwd=std::move(buffer);
    }
    return cwd;
}

