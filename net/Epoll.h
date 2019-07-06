//
// Created by zhsy on 19-6-8.
//

#ifndef HCCSERVER_EPOLL_H
#define HCCSERVER_EPOLL_H

#define MAXEPOLLSIZE 10000

#include <sys/epoll.h>
#include <map>
#include <memory>
#include <vector>
#include <iostream>
#include "Channel.h"


class Epoll {
public:
    typedef std::shared_ptr<Channel>  ptrChannel;
    typedef std::vector<ptrChannel> VectorCh;

    typedef std::map<int,ptrChannel> ChannelPool;

    Epoll(int size=MAXEPOLLSIZE);
    ~Epoll();

    bool addChannel(ptrChannel channel);
    bool removeChannel(ptrChannel channel);
    int updateChannel(ptrChannel channel);

    VectorCh poll();


private:
    int _waitFd;

    struct epoll_event _events[MAXEPOLLSIZE];
    ChannelPool channelPool;
};


#endif //HCCSERVER_EPOLL_H
