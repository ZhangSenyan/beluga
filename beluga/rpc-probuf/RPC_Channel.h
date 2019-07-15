//
// Created by zhsy on 19-7-15.
//

#ifndef BELUGA_RPC_CHANNEL_H
#define BELUGA_RPC_CHANNEL_H
#include <google/protobuf/service.h>
#include <google/protobuf/stubs/common.h>
#include <mutex>
#include <condition_variable>

#include "beluga/TCPClient.h"
class EventLoop;

class RPC_Channel:public ::google::protobuf::RpcChannel{
public:
    RPC_Channel(char * ip, int port,EventLoop* eventLoop);
    void handle_read();
    void handle_write();
    void handle_error();
    void write(const std::string& str);
    std::string& read();
    virtual void CallMethod(const ::google::protobuf::MethodDescriptor* method,
                            ::google::protobuf::RpcController* ,
                            const ::google::protobuf::Message* request,
                            ::google::protobuf::Message* response,
                            ::google::protobuf::Closure*) ;
private:
    std::shared_ptr<Channel> _channel;
    std::string _sendMsg;
    std::string _recvMsg;
    bool _requesting;
    EventLoop* _eventLoop;
    TCPClient _tcpClient;
    std::mutex _mutex;
    std::condition_variable _cv;
};





#endif //BELUGA_RPC_CHANNEL_H
