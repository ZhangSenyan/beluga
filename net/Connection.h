//
// Created by zhsy on 19-6-29.
//

#ifndef HCCSERVER_CONNECTION_H
#define HCCSERVER_CONNECTION_H
#include <iostream>
#include <_G_config.h>
#include <functional>
#include <arpa/inet.h>
#include <queue>
#include <memory>
#include <string>

class Channel;
class CellTask;

#include "Buffer.h"

class AcceptThread;

struct Connection:std::enable_shared_from_this<Connection>{
public:
    typedef std::shared_ptr<Connection> ConnPtr;
    Connection(int fd,struct sockaddr_in clientAddr);
    ~Connection();
    int getFd();
    void setFd(int fd);
    void handleRead();
    void handleWrite();
    void handleError();
    std::string getIP();
    int getPort();
    std::shared_ptr<Channel> getChannel();
    int flushBuffer();
    std::shared_ptr<AcceptThread> getAcceptThread();
    void openListenEvent();
    void closeListenEvent();
    int writeBuffer(std::string result);
    void setAcceptThread(std::shared_ptr<AcceptThread> acceptThread);
private:
    typedef std::function<void()> Handler;
    int _fd;
    struct sockaddr_in _clientAddr;
    size_t _expiredTime;
    std::shared_ptr<Channel> _channel;
    std::weak_ptr<AcceptThread> _acceptThread;  //智能相互引用问题
    Buffer _buffer;
};


#endif //HCCSERVER_CONNECTION_H
