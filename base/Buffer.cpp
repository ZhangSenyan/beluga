//
// Created by zhsy on 19-7-2.
//

#include "Buffer.h"
#include <iostream>
#include <cstring>
#include <cstdint>
#include <arpa/inet.h>
#include <string>
#include <vector>
#include <stdlib.h>
#include "Util.h"
#include <stdint.h>


#define BUFFER_SIZE 4096
Buffer::Buffer(int fd):_fd(fd),recvIndexEnd(0),sendIndexEnd(0){
    recvbuffer=new char[BUFFER_SIZE];
    sendbuffer=new char[BUFFER_SIZE];

}
Buffer::~Buffer(){
    delete recvbuffer;
    delete sendbuffer;
}
/*
 * 返回值为0,代表写入成功
 * 返回值为-1，代表写入失败
 * */
int Buffer::write(std::string str){
    std::cout<<"Buffer write str"<<std::endl;
    write((char*)str.c_str(),str.length());
    return 0;
}

int Buffer::write(const char* str){
    //获取写入字符串长度
    uint32_t len=strlen(str);
    return write(str,len);
}

int Buffer::write(const char* str,uint32_t len){
    if(len>BUFFER_SIZE-sendIndexEnd)
        return -1;

    //设置头端
    uint32_t nlen=htonl(len);
    memcpy(sendbuffer+sendIndexEnd,&nlen,4);
    sendIndexEnd+=4;

    //拷贝数据
    memcpy(sendbuffer+sendIndexEnd,str,len);
    sendIndexEnd+=len;


    return 0;
}
int Buffer::flushSend(){
    std::cout<<"Buffer flushSend()"<<std::endl;
    if(sendIndexEnd==0){
        //当前缓冲区为空
        return 0;
    }


    std::cout<<sendbuffer<<std::endl;
    int ret=writen(_fd,sendbuffer,(size_t )sendIndexEnd);

    if(ret==-1)
        //写入0字节
        return -1;

    //将剩余的字符向前移动
    memcpy(sendbuffer,sendbuffer+ret,sendIndexEnd-ret);
    sendIndexEnd-=ret;
    return 0;
}

std::vector<std::string> Buffer::readStream(){
    std::cout<<"readStream()"<<std::endl;
    std::vector<std::string> retV;
    int retn=readn(_fd,recvbuffer+recvIndexEnd,BUFFER_SIZE-recvIndexEnd);
    std::cout<<"retn= "<<retn<<std::endl;
    if(retn==-1)
        return retV;
    recvIndexEnd+=retn;
    int recvIndexBegin=0;
    while(recvIndexBegin<recvIndexEnd){
        uint32_t len=ntohl(*((uint32_t*)(recvbuffer+recvIndexBegin)));
        recvIndexBegin+=4;
        if(len>recvIndexEnd-recvIndexBegin)
            break;
        retV.push_back(std::string(recvbuffer+recvIndexBegin,recvbuffer+recvIndexBegin+len));
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