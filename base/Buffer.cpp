//
// Created by zhsy on 19-5-2.
//

#include "Buffer.h"
#include <iostream>
#include <cstring>
#include <cstdint>
#include <arpa/inet.h>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstdint>

#include "Util.h"
#include "Connection.h"

#define BUFFER_SIZE 4096
Buffer::Buffer(int fd,Connection *holder):_fd(fd),recvIndexEnd(0),sendIndexEnd(0),_holder(holder){
    recvbuffer=new char[BUFFER_SIZE];
    sendbuffer=new char[BUFFER_SIZE];

}
Buffer::~Buffer(){
    std::cout<<"~Buffer()"<<std::endl;
    delete recvbuffer;
    delete sendbuffer;
}
/*
 * 返回值为0,代表写入成功
 * 返回值为-1，代表写入失败
 * */
int Buffer::write(std::string str){

    write((char*)str.c_str(),str.length());
    return 0;
}

int Buffer::write(const char* str){
    //获取写入字符串长度
    uint32_t len=strlen(str);
    return write(str,len);
}

int Buffer::write(const char* str,uint32_t len){
    MutexLockGuard lock(_mutex);
    //std::cout<<"write()"<<std::endl;
    if(len+4>BUFFER_SIZE-sendIndexEnd)
        return -1;
    //设置头端
    uint32_t nlen=htonl(len);
    memcpy(sendbuffer+sendIndexEnd,&nlen,4);
    sendIndexEnd+=4;

    //拷贝数据
    memcpy(sendbuffer+sendIndexEnd,str,len);
    sendIndexEnd+=len;

    //定量发送，只有累计数据大于BUFFER一半时，才开始发送数据
    if(sendIndexEnd>BUFFER_SIZE/2&&_holder){
        _holder->openListenEvent();
    }

    return 0;
}
/*
 * 如果为空    返回0
 * 如果出错    返回-1
 * 成功发送    返回发送的字节数
 *
 * 需要加锁，避免读得冲刷缓冲区时写入数据
 *
 * */
int Buffer::flushSend(){
    //std::cout<<"flushSend()"<<std::endl;
    MutexLockGuard lock(_mutex);

    if(sendIndexEnd==0){
        //当前缓冲区为空
        //std::cout<<"Buffer flushSend()  num="<<0<<std::endl;
        return 0;
    }

    int ret=writen(_fd,sendbuffer,(size_t )sendIndexEnd);
    //std::cout<<"Buffer flushSend()  num="<<ret<<std::endl;
    if(ret==-1)
        //写入0字节
        return -1;

    //将剩余的字符向前移动
    memcpy(sendbuffer,sendbuffer+ret,sendIndexEnd-ret);
    sendIndexEnd-=ret;

    return ret;
}
bool Buffer::empty() {
    return 0==sendIndexEnd;
}
/*
 * 读写缓冲区分离
 * 读的时候不需要加锁
 * */
std::vector<std::string> Buffer::readStream(){
    //std::cout<<"readStream()"<<std::endl;
    std::vector<std::string> retV;
    int retn=readn(_fd,recvbuffer+recvIndexEnd,BUFFER_SIZE-recvIndexEnd);
    if(retn==-1)
        return retV;
    recvIndexEnd+=retn;
    int recvIndexBegin=0;
    while(recvIndexBegin<recvIndexEnd){
        uint32_t len=ntohl(*((uint32_t*)(recvbuffer+recvIndexBegin)));
        recvIndexBegin+=4;
        if(len>recvIndexEnd-recvIndexBegin)
            break;
        retV.emplace_back(std::string(recvbuffer+recvIndexBegin,recvbuffer+recvIndexBegin+len));
        recvIndexBegin+=len;
    }
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
    //std::cout<<"readStimple()"<<std::endl;

    int retn=readn(_fd,recvbuffer,BUFFER_SIZE);
    if(retn==-1)
        return "";
    return std::string(recvbuffer,recvbuffer+retn);
}