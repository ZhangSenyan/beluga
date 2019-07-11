//
// Created by zhsy on 19-7-6.
//

#ifndef HCCSERVER_CLIENT_H
#define HCCSERVER_CLIENT_H

#include "Buffer.h"
class Client {
public:
    Client();
    ~Client();
    int getFD();
    void write(std::string str);
    size_t readAndDeal();
private:
    int _fd;
    Buffer _buffer;
};


#endif //HCCSERVER_CLIENT_H
