//
// Created by zhsy on 19-6-27.
//

#ifndef HCCSERVER_REF_H
#define HCCSERVER_REF_H




#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/epoll.h>

#define MAX_SIZE 1024

enum EventType
{
    EIN = EPOLLIN,		  // 读事件
    EOUT = EPOLLOUT,	  // 写事件
    ECLOSE = EPOLLRDHUP,  // 对端关闭连接或者写半部
    EPRI = EPOLLPRI,	  // 紧急数据到达
    EERR = EPOLLERR,	  // 错误事件
    EET = EPOLLET, 		  // 边缘触发
    EDEFULT = EIN | ECLOSE | EERR | EET
};


class CEvent
{
public:
    CEvent();
    ~CEvent();
    int Register_event(int fd,     EventType type = EDEFULT);
    int unRegister_event(int fd);
    void* EventHandle(void* arg);
    void SetNoblocking(int v_sockfd);

private:
    int epfd;
    bool is_running;
    pthread_t  m_tid;
    struct events[EPOLL_SIZE];
    CThreadPoolProxy *pool;
};

CEvent::CEvent()
{

    epfd = epoll_create(MAX_SIZE);
    if(epfd == -1)
    {
        printf("epoll_create failed.");
        return -1;
    }
    pthread_t tid = 0;
    pthread_create(&tid, NULL, EventHandle, (void*)this == 0);
    m_tid = tid;
    //线程池初始化
    pool = CThreadPoolProxy::instance();

}


CEvent::~CEvent()
{
    if(pthread_cancel(m_tid) == 0)
    {
        pthread_join(m_tid, (void **)NULL);
    }
}



void CEvent::SetNoblocking(int v_sockfd)
{
    int opts = fcntl(v_sockfd,F_GETFL);
    if(opts < 0)
    {
        printf("fcntl(sockfd, F_GETFL) failed.");
        opts = opts|O_NONBLOCK;
    }
    fcntl(v_sockfd, F_SETFL, opts);

}






int CEvent::Register_event(int fd, EventType type = EDEFULT)
{
    SetNoblocking(fd);
    struct epoll_event ev;
    ev.data.fd = fd
    ev.events = type;
    if(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1)
    {
        printf("epoll_ctl: EPOLL_CTL_ADD failed, fd[%d].",&fd);
        return -1;
    }
    return 0;
}


int CEvent::unRegister_event(int fd)
{
    if(epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL) == -1)
    {
        printf("epoll_ctl: EPOLL_CTL_DEL failed, fd[%d].",&fd);
        return -1;
    }
    return 0;
}


void* CEvent::EventHandle(void* arg)
{

    CEvent &event = *(CEvent*)arg;
    while(is_running)
    {
        int ret = epoll_wait(event.epfd, event.events, MAX_SIZE, -1);
        if(ret < 0)
        {
            printf("epoll_wait failed, epfd[%d]",&event.epfd);
        }
        for(int i=0; i<ret; i++)
        {
            int connfd = event.events[i].data.fd;

            if(event.events[i].events & EPOLLIN)
            {
                CTask* ta=new CMyTask;       //  具体的方法自己实现。
                ta->SetConnFd(connfd);
                pool->AddTask(ta);

            }
        }
    }



}







#endif //HCCSERVER_REF_H
