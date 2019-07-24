/**
 * @author Zhang Senyan
 * Date: 2019-04-15
 *
 */

#ifndef BELUGA_CONNECTION_H
#define BELUGA_CONNECTION_H

#include <iostream>
#include <_G_config.h>
#include <functional>
#include <arpa/inet.h>
#include <queue>
#include <memory>
#include <string>

#include "beluga/base/Buffer.h"


class Channel;
class CellTask;
class AcceptThread;

struct Connection:std::enable_shared_from_this<Connection>{
public:
    typedef std::shared_ptr<Connection> ConnPtr;

    Connection(int fd,struct sockaddr_in clientAddr);
    ~Connection();

    //获取Connection对应的 fd
    int getFd();
    void setFd(int fd);

    //事件处理函数
    void handleRead();
    void handleWrite();
    void handleError();

    //获取客户端 IP地址 port信息
    std::string getIP() const;
    int getPort()const;

    std::shared_ptr<Channel> getChannel();

    //将缓冲区数据发送至网络
    int flushBuffer();

    std::shared_ptr<AcceptThread> getAcceptThread();

    //监听客户端是否可写
    void openListenEvent();
    void closeListenEvent();

    //将数据写入缓冲区
    int writeBuffer(std::string result);

    void setAcceptThread(std::shared_ptr<AcceptThread> acceptThread);

private:

    int _fd;
    struct sockaddr_in _clientAddr;
    size_t _expiredTime;
    std::shared_ptr<Channel> _channel;
    std::weak_ptr<AcceptThread> _acceptThread;  //智能相互引用问题
    Buffer _buffer;
};


#endif //BELUGA_CONNECTION_H
