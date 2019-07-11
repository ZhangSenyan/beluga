//
// Created by zhsy on 19-5-2.
//

#ifndef HCCSERVER_BUFFER_H
#define HCCSERVER_BUFFER_H


#include <memory>
#include <vector>
#include <string>
#include <queue>
#include "MutexLock.h"
class Connection;
class Buffer {
public:
    explicit Buffer(int fd,Connection* holder= nullptr);
    ~Buffer();
    /*
     * 返回值为0,代表写入成功
     * 返回值为-1，代表写入失败
     * */
    int write(std::string str);
    int write(const char* str);
    int write(const char* str,uint32_t len);
    int flushSend();
    bool empty();
    std::vector<std::string> readStream();
    std::string readSimple();
private:
    size_t recvIndexEnd;
    size_t sendIndexEnd;
    char *recvbuffer;
    char *sendbuffer;
    int _fd;
    Connection* _holder;
    mutable MutexLock _mutex;
    std::queue<std::string> waitQ;
};


#endif //HCCSERVER_BUFFER_H
