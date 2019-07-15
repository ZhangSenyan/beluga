//
// Created by zhsy on 19-6-16.
//

#ifndef HCCSERVER_CHANNEL_H
#define HCCSERVER_CHANNEL_H


#include <functional>
#include <sys/epoll.h>
#include <memory>
#include "beluga/beluga.h"
class Connection;

class Channel{

public:
    
    explicit Channel(int fd);
    ~Channel();
private:
    beluga::CallFunc _readhandler;
    beluga::CallFunc _writehandler;
    beluga::CallFunc _errorhandler;
    int _fd;
    __uint32_t _events;
    __uint32_t _revents;
    std::weak_ptr<Connection> _holder;

public:
    void setHolder(const std::shared_ptr<Connection> &holder);
    void setReadHandler(beluga::CallFunc readhandler);
    void setWriteHandler(beluga::CallFunc writehandler);
    void setErrorHandler(beluga::CallFunc errorhandler);
    void handleEvents();
    int getFd();
    void setFD(int fd);
    __uint32_t getEvents();
    void setEvents(__uint32_t events);
    __uint32_t getRevents();
    void setRevents(__uint32_t revents);
    void addEvents(__uint32_t events);
    void removeEvents(__uint32_t events);
    void updateEvents();
    bool expired();
};


#endif //HCCSERVER_CHANNEL_H




