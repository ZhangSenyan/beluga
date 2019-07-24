/**
 * @author Zhang Senyan
 * Date: 2019-06-27
 *
 * RPC 请求端 channel
 */

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

    /**
     * 构造函数
     * @param ip  服务器所属的ip地址
     * @param port  服务器监听的端口号
     * @param eventLoop  所依附的 eventloop
     */
    RPC_Channel(char * ip, int port,EventLoop* eventLoop);

    //回调函数
    void handle_read();
    void handle_write();
    void handle_error();

    //读写缓冲区
    void write(const std::string& str);
    std::string& read();

    //重写 ::google::protobuf::RpcChannel::CallMethod
    virtual void CallMethod(const ::google::protobuf::MethodDescriptor* method,
                            ::google::protobuf::RpcController* ,
                            const ::google::protobuf::Message* request,
                            ::google::protobuf::Message* response,
                            ::google::protobuf::Closure*);
private:

    std::shared_ptr<Channel> _channel;

    std::string _sendMsg;

    std::string _recvMsg;

    bool _requesting;

    //所依附的eventloop
    EventLoop* _eventLoop;

    //客户端
    TCPClient _tcpClient;

    std::mutex _mutex;

    std::condition_variable _cv;
};





#endif //BELUGA_RPC_CHANNEL_H
