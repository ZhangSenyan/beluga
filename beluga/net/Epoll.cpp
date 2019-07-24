#include "Epoll.h"
#include <cstring>
#include <cassert>
//
// Created by zhsy on 19-6-8.
//

Epoll::Epoll(int size):_waitFd(epoll_create(size)),_events(){

}

Epoll::~Epoll(){

}

bool Epoll::addChannel(ptrChannel channel){

    channelPool[channel->getFD()] = channel;
    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.events = channel->getEvents();
    ev.data.fd = channel->getFD();

    if (epoll_ctl(_waitFd, EPOLL_CTL_ADD, channel->getFD(), &ev) < 0)
    {
        fprintf(stderr, "epoll set insertion error: fd=%d\n", channel->getFD());
        return -1;
    }

}
int Epoll::updateChannel(ptrChannel channel){

    channelPool[channel->getFD()] = channel;
    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.events = channel->getEvents();
    ev.data.fd = channel->getFD();
    //std::cout<<"Epoll::updateChannel: FD="<<ev.data.fd<<" events="<<ev.events<<std::endl;
    if (epoll_ctl(_waitFd, EPOLL_CTL_MOD, channel->getFD(), &ev) < 0)
    {
        fprintf(stderr, "epoll set modify error: fd=%d\n", channel->getFD());
        return -1;
    }

}

bool Epoll::removeChannel(ptrChannel channel){
    channelPool.erase(channel->getFD());
    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.events = 0;
    ev.data.fd = channel->getFD();

    if (epoll_ctl(_waitFd, EPOLL_CTL_DEL, channel->getFD(), &ev) < 0)
    {
        fprintf(stderr, "epoll set delete error: fd=%d\n", channel->getFD());
        return -1;
    }
}


Epoll::VectorCh Epoll::poll(){
    int nfds = epoll_wait(_waitFd, _events, 100, -1);
    if (nfds == -1)
    {
        perror("epoll_wait");
    }
    VectorCh v;  //内置指针不能隐式转化为智能指针

    for(int i=0;i<nfds;i++){
        int readfd=_events[i].data.fd;
        assert(channelPool.find(readfd)!=channelPool.end());
        channelPool[readfd]->setRevents(_events[i].events);
        v.push_back(channelPool[readfd]);
    }
    return v;
}
bool Epoll::find(ptrChannel channel){
    return channelPool.find(channel->getFD())!=channelPool.end();
}