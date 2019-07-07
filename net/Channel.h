//
// Created by zhsy on 19-6-16.
//

#ifndef HCCSERVER_CHANNEL_H
#define HCCSERVER_CHANNEL_H


#include <functional>
#include <sys/epoll.h>
#include <memory>
class Connection;

class Channel{

public:
    typedef std::function<void(void)> CallFunc;
    Channel(int fd);
    ~Channel();
private:
    CallFunc _readhandler;
    CallFunc _writehandler;
    CallFunc _errorhandler;
    int _fd;
    __uint32_t _events;
    __uint32_t _revents;
    std::weak_ptr<Connection> _holder;

public:
    void setHolder(const std::shared_ptr<Connection> &holder);
    void setReadHandler(CallFunc readhandler);
    void setWriteHandler(CallFunc writehandler);
    void setErrorHandler(CallFunc errorhandler);
    void handleEvents();
    int getFd();
    __uint32_t getEvents();
    void setEvents(__uint32_t events);
    __uint32_t getRevents();
    void setRevents(__uint32_t revents);
    void addEvents(__uint32_t events);
    void removeEvents(__uint32_t events);
    void updateEvents();
};


#endif //HCCSERVER_CHANNEL_H




