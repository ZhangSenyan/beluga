/**
 * @author Zhang Senyan
 * Date: 2019-04-21
 *
 * Buffer类：为数据读写提供缓冲区
 */

#ifndef BELUGA_BUFFER_H
#define BELUGA_BUFFER_H

#include <memory>
#include <vector>
#include <string>
#include <queue>

#include "beluga/base/MutexLock.h"

class Connection;

class Buffer {
public:

    //构造函数
    explicit Buffer(int fd,Connection* holder= nullptr);

    //析构函数
    ~Buffer();

    /**
     * Buffer写入函数
     * 仅仅写入内存，通过flushSend()函数才会真正向外发送
     */
    int write(std::string str);
    int write(const char* str);
    int write(const char* str,uint32_t len);

    /**
     * Function： 将缓冲区的数据送入网络
     * 在定时器 的 timehandler 或者 Connection的 writeHandler 里调用
     */
    int flushSend();

    //查看缓冲区是否为空
    bool empty();

    //读取缓冲区数据，逐条输出
    std::vector<std::string> readStream();

    //简易读取函数
    std::string readSimple();

private:
    //接收缓冲区游标
    size_t recvIndexEnd;

    //发送缓冲区游标
    size_t sendIndexEnd;

    //接收缓冲区
    char *recvbuffer;

    //发送缓冲区
    char *sendbuffer;

    //所属Connection的文件描述符
    int _fd;

    //所属Connection
    Connection* _holder;

    //缓冲区锁
    mutable MutexLock _mutex;

    //待写入数据的等待队列
    //当缓冲区满时使用
    std::queue<std::string> waitQ;
};


#endif //BELUGA_BUFFER_H
