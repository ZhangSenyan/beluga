/**
 * @author Zhang Senyan
 * Date: 2019-04-13
 *
 */

#ifndef BELUGA_CHANNEL_H
#define BELUGA_CHANNEL_H


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

    //事件处理函数
    beluga::CallFunc _readhandler;
    beluga::CallFunc _writehandler;
    beluga::CallFunc _errorhandler;

    //所属Connection的文件句柄
    int _fd;


    __uint32_t _events;

    __uint32_t _revents;

    //所属的Connection
    std::weak_ptr<Connection> _holder;

public:

    //关联所属Connection
    void setHolder(const std::shared_ptr<Connection> &holder);

    //注册事件处理函数
    void setReadHandler(beluga::CallFunc readhandler);
    void setWriteHandler(beluga::CallFunc writehandler);
    void setErrorHandler(beluga::CallFunc errorhandler);

    void handleEvents();

    int getFD();
    void setFD(int fd);

    __uint32_t getEvents();

    void setEvents(__uint32_t events);

    //获取返回事件
    __uint32_t getRevents();

    //设定返回事件
    void setRevents(__uint32_t revents);

    void addEvents(__uint32_t events);

    void removeEvents(__uint32_t events);


    void updateEvents();
    bool expired();
};


#endif //BELUGA_CHANNEL_H




