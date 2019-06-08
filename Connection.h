//
// Created by zhsy on 19-6-8.
//

#ifndef HCCSERVER_CONNECTION_H
#define HCCSERVER_CONNECTION_H

#include <iostream>
#include <_G_config.h>
#include <functional>
#include <arpa/inet.h>
#include "CellTask.h"
#include <queue>
#include <memory>


class Connection {
public:

    Connection();
    ~Connection();
    int getFd();
    void setFd(int fd);
    void handleRead();
    void handleWrite();

private:
    typedef std::function<void()> Handler;
    typedef std::shared_ptr<std::queue<std::shared_ptr<CellTask>>> PtrTaskQueue;
    int _fd;
    __uint32_t  events;
    __uint32_t  revents;
    struct sockaddr_in clientAddr;
    size_t expiredTime;
    PtrTaskQueue ptrTaskQueue;
};


#endif //HCCSERVER_CONNECTION_H
