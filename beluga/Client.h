//
// Created by zhsy on 19-7-6.
//

#ifndef HCCSERVER_CLIENT_H
#define HCCSERVER_CLIENT_H

#include "beluga/base/Buffer.h"
class Client {
public:
    Client(char * ip, int port);
    ~Client();
    int getFD();
    void write(std::string str);
    std::vector<std::string> readStream();
private:
    int _fd;

    Buffer _buffer;
};


#endif //HCCSERVER_CLIENT_H
