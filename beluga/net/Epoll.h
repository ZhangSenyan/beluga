/**
 * @author Zhang Senyan
 * Date: 2019-04-09
 *
 */

#ifndef BELUGA_EPOLL_H
#define BELUGA_EPOLL_H

#define MAXEPOLLSIZE 10000

#include <sys/epoll.h>
#include <map>
#include <memory>
#include <vector>
#include <iostream>

#include "beluga/net/Channel.h"

class Epoll {
public:
    typedef std::shared_ptr<Channel>  ptrChannel;
    typedef std::vector<ptrChannel> VectorCh;
    typedef std::map<int,ptrChannel> ChannelPool;

    /**
     * 构造函数
     * @param size 所允许最大连接数量
     */
    explicit Epoll(int size=MAXEPOLLSIZE);
    ~Epoll();


    bool addChannel(ptrChannel channel);

    bool removeChannel(ptrChannel channel);

    /**
     * 当channel中所监听的事件变化时调用
     */
    int updateChannel(ptrChannel channel);

    /**
     * poll and wait
     * @return 上一时刻触发的事件集合
     */
    VectorCh poll();

    bool find(ptrChannel channel);

private:

    int _waitFd;

    struct epoll_event _events[MAXEPOLLSIZE];

    //注册该epoll所负载的所有事件
    ChannelPool channelPool;
};


#endif //BELUGA_EPOLL_H
