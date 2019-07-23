/**
 * @author Zhang Senyan
 * Date: 2019-04-21
 */

#include "Buffer.h"
#include <iostream>
#include <cstring>
#include <cstdint>
#include <arpa/inet.h>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstdint>

#include "beluga/base/Util.h"
#include "beluga/net/Connection.h"

//定义Buffer缓冲区的大小
#define BUFFER_SIZE 16384

//构造函数
Buffer::Buffer(int fd,Connection *holder):_fd(fd),recvIndexEnd(0),sendIndexEnd(0),_holder(holder){
    recvbuffer=new char[BUFFER_SIZE];
    sendbuffer=new char[BUFFER_SIZE];
}

//析构函数
Buffer::~Buffer(){
    delete recvbuffer;
    delete sendbuffer;
}

/**
 * Buffer写入函数
 * @param str
 *        type: std::string
 */
int Buffer::write(const std::string str){

    //调用 Buffer::write(const char* str,uint32_t len);
    write((char*)str.c_str(),str.length());

    return 0;
}


/**
 * Buffer写入函数
 * @param str
 *        type: char*
 */
int Buffer::write(const char* str){

    //获取写入字符串长度
    uint32_t len=strlen(str);

    //调用 Buffer::write(const char* str,uint32_t len);
    return write(str,len);
}

/**
 * Buffer写入函数
 * @param str 写入字符串的地址
 * @param len 写入字符串的长度
 *
 * @return -1  写入失败
 *         0   写入成功
 */
int Buffer::write(const char* str,uint32_t len){

    MutexLockGuard lock(_mutex);

    /**
     * 判断缓冲区剩余空间是否足够
     * len+4 ： 写入数据的长度 + 数据头长度
     * sendIndexEnd ： 写入区尾部指针
     */
    if(len+4>BUFFER_SIZE-sendIndexEnd)
        return -1;

    //主机字节序转网络字节序
    uint32_t nlen=htonl(len);

    //写入包头，指针后移四个字节
    memcpy(sendbuffer+sendIndexEnd,&nlen,4);
    sendIndexEnd+=4;

    //拷贝数据，指针后移len个字节
    memcpy(sendbuffer+sendIndexEnd,str,len);
    sendIndexEnd+=len;

    /**
     * 定量发送
     * 判断当期写入的数据是否大于缓冲区的一半
     * _holder ： 拥有 Buffer的Connection
     */
    if(sendIndexEnd>BUFFER_SIZE/2&&_holder){

        //Connection 开始监听写入事件
        _holder->openListenEvent();

    }

    return 0;
}
/**
 * Function： 冲刷发送缓冲区
 * 即网络可以写入时，将缓冲区的数据发送到网络上
 *
 * @return -1   发送失败
 *          0   当前缓冲区为空，发送0字节
 *         >0   实际发送的字节数
 */
int Buffer::flushSend(){

    // 在冲刷缓冲区的时候，不允许其他进程写入数据
    MutexLockGuard lock(_mutex);

    if(sendIndexEnd==0){
        //当前缓冲区为空
        return 0;
    }

    // 将缓冲区数据写入网络
    int ret=writen(_fd,sendbuffer,(size_t )sendIndexEnd);

    if(ret==-1)
        //写入失败
        return -1;

    /**
     * 将缓冲区剩余的数据移动到缓冲区前
     * 方便以后到来的数据追加到缓冲区的尾部
     * 调整游标：sendIndexEnd
     */
    memcpy(sendbuffer,sendbuffer+ret,sendIndexEnd-ret);
    sendIndexEnd-=ret;

    return ret;
}

//判断当前缓冲区是否为空
bool Buffer::empty() {
    return 0==sendIndexEnd;
}

/**
 * Function： 读取缓冲区数据
 * 当数据到来时，先将数据读入缓冲区，然后解析出各条数据
 * 由于读写缓冲区分离，所以数据读取时和写入可以同时进行，不需要加锁
 *
 * @return std::vector<std::string>
 *         消息的集合，里面每条string代表一条消息
 */

std::vector<std::string> Buffer::readStream(){

    std::vector<std::string> retV;

    //读取消息缓冲区
    int retn=readn(_fd,recvbuffer+recvIndexEnd,BUFFER_SIZE-recvIndexEnd);
    if(retn==-1)
        //错误，返回空
        return retV;

    //移动游标
    recvIndexEnd+=retn;

    //逐条解析数据，直到前后游标相遇
    int recvIndexBegin=0;
    while(recvIndexBegin<recvIndexEnd){

        //获取数据包长度，并转换为主机字节序
        uint32_t len=ntohl(*((uint32_t*)(recvbuffer+recvIndexBegin)));


        if(len>recvIndexEnd-recvIndexBegin){
            //收到的数据不完整，不能凑够一条
            recvIndexBegin-=4;
            break;
        }

        //生成一条数据，移动游标
        retV.emplace_back(std::string(recvbuffer+recvIndexBegin,recvbuffer+recvIndexBegin+len));
        recvIndexBegin+=len;
    }

    /**
     * 1. 如果数据已经被完全读出，直接将游标移动到头部
     * 2. 如果数据未读出，需要将剩余的部分数据移动到头部
     */
    if(recvIndexBegin==recvIndexEnd){
        recvIndexEnd=0;
    }
    else{
        memcpy(recvbuffer,recvbuffer+recvIndexBegin,recvIndexEnd-recvIndexBegin);
        recvIndexEnd-=recvIndexBegin;
    }
    return retV;
}

std::string Buffer::readSimple(){

    int retn=readn(_fd,recvbuffer,BUFFER_SIZE);
    if(retn==-1)
        return "";
    return std::string(recvbuffer,recvbuffer+retn);
}