//
// Created by zhsy on 19-6-8.
//

#ifndef HCCSERVER_EPOLL_H
#define HCCSERVER_EPOLL_H

#define MAXEPOLLSIZE 10000

#include <sys/epoll.h>
#include <map>
#include "Channel.h"
#include <memory>
#include <vector>
#include <iostream>


class Epoll {
public:
    typedef std::shared_ptr<Channel>  ptrChannel;
    typedef std::vector<ptrChannel> VectorCh;
    typedef std::shared_ptr<VectorCh> ptrVectorCh;
    typedef std::map<int,ptrChannel> ChannelPoll;

    Epoll(int size=MAXEPOLLSIZE);
    ~Epoll();

    bool addChannel(ptrChannel channel);
    bool removeChannel(ptrChannel);

    ptrVectorCh poll();


private:
    int _waitFd;

    struct epoll_event _events[MAXEPOLLSIZE];
    ChannelPoll channelPoll;
};


#endif //HCCSERVER_EPOLL_H
