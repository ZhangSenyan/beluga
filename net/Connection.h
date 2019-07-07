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
    Connection(int fd,struct sockaddr_in clientAddr,AcceptThread *acceptThread);
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
    AcceptThread* getAcceptThread();
    void openListenEvent();
    void closeListenEvent();
    int writeBuffer(std::string result);
private:
    typedef std::function<void()> Handler;
    int _fd;
    struct sockaddr_in _clientAddr;
    size_t _expiredTime;
    std::shared_ptr<Channel> _channel;
    AcceptThread* _acceptThread;
    Buffer _buffer;
};


#endif //HCCSERVER_CONNECTION_H
