/**
 * @author Zhang Senyan
 * Date: 2019-05-15
 *
 * TCP Client
 */

#ifndef BELUGA_CLIENT_H
#define BELUGA_CLIENT_H

#include "beluga/base/Buffer.h"

class Channel;

class TCPClient {

public:
    /**
     * 构造函数： 服务器 IP 和 端口号
     * @param ip
     * @param port
     */
    TCPClient(const char * ip, int port);
    ~TCPClient();

    int getFD();

    //发送消息
    void write(std::string str);

    void setBlocking();

    //读取消息
    std::vector<std::string> readStream();

private:
    int _fd;
    Buffer _buffer;
    
};


#endif //HCCSERVER_CLIENT_H
