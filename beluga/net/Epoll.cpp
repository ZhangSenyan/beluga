/**
 * @author Zhang Senyan
 * Date: 2019-04-09
 *
 */

#include "Epoll.h"
#include <cstring>
#include <cassert>

/**
 * 构造函数
 * @param size 最大运行负载连接数
 */
Epoll::Epoll(int size):
    _waitFd(epoll_create(size)),
    _events(){

}

Epoll::~Epoll(){

}

/**
 * 添加 Connection
 * @param channel  ： 与Connection对应
 * @return false : 添加失败
 */
bool Epoll::addChannel(ptrChannel channel){

    //将对应事件注册到池中
    channelPool[channel->getFD()] = channel;

    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.events = channel->getEvents();
    ev.data.fd = channel->getFD();

    //将事件对应的 fd 添加到epoll中
    if (epoll_ctl(_waitFd, EPOLL_CTL_ADD, channel->getFD(), &ev) < 0)
    {
        fprintf(stderr, "epoll set insertion error: fd=%d\n", channel->getFD());
        return false;
    }
    return true;
}

/**
 * 更新 epoll 中对应fd的监听事件
 * @param channel
 * @return -1 ： 更新失败
 */
int Epoll::updateChannel(ptrChannel channel){

    channelPool[channel->getFD()] = channel;

    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.events = channel->getEvents();
    ev.data.fd = channel->getFD();

    // 更改epoll中对应fd的监听事件
    if (epoll_ctl(_waitFd, EPOLL_CTL_MOD, channel->getFD(), &ev) < 0)
    {
        fprintf(stderr, "epoll set modify error: fd=%d\n", channel->getFD());
        return -1;
    }
    return 0;
}

/**
 * 注销 Connection
 * @param channel  ： 与Connection对应
 * @return false : 注销失败
 *         ture ： 注销成功
 */
bool Epoll::removeChannel(ptrChannel channel){

    channelPool.erase(channel->getFD());

    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.events = 0;
    ev.data.fd = channel->getFD();

    //删除epoll中对应的文件描述符
    if (epoll_ctl(_waitFd, EPOLL_CTL_DEL, channel->getFD(), &ev) < 0)
    {
        fprintf(stderr, "epoll set delete error: fd=%d\n", channel->getFD());
        return false;
    }

    return true;
}

/**
 * 等待事件返回
 * @return std::vector<ptrChannel>
 *         所触发的 channel集合，以vector形式返回
 */
Epoll::VectorCh Epoll::poll(){

    //等待事件返回
    int nfds = epoll_wait(_waitFd, _events, 100, -1);
    if (nfds == -1)
    {
        perror("epoll_wait");
    }

    VectorCh v;  //内置指针不能隐式转化为智能指针

    for(int i=0;i<nfds;i++){
        int readfd=_events[i].data.fd;
        assert(channelPool.find(readfd)!=channelPool.end());

        // 取出channel，设定返回事件
        channelPool[readfd]->setRevents(_events[i].events);
        v.push_back(channelPool[readfd]);
    }
    return std::move(v);
}


bool Epoll::find(ptrChannel channel){
    return channelPool.find(channel->getFD())!=channelPool.end();
}