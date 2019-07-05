//
// Created by zhsy on 19-7-2.
//

#ifndef HCCSERVER_BUFFER_H
#define HCCSERVER_BUFFER_H


#include <memory>
#include <vector>

class Buffer {
public:
    Buffer(int fd);
    ~Buffer();
    /*
     * 返回值为0,代表写入成功
     * 返回值为-1，代表写入失败
     * */
    int write(std::string str);
    int write(const char* str);
    int write(const char* str,uint32_t len);
    int flushSend();

    std::vector<std::string> readStream();
private:
    size_t recvIndexEnd;
    size_t sendIndexEnd;
    char *recvbuffer;
    char *sendbuffer;
    int _fd;
};


#endif //HCCSERVER_BUFFER_H
