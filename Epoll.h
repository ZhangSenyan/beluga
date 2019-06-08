//
// Created by zhsy on 19-6-8.
//

#ifndef HCCSERVER_EPOLL_H
#define HCCSERVER_EPOLL_H

#include <sys/epoll.h>

class Epoll {
public:
    Epoll(int size=10000):_waitFd(epoll_create(size)){

    }
    ~Epoll();

private:
    int _waitFd;
};


#endif //HCCSERVER_EPOLL_H
