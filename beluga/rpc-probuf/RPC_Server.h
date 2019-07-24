/**
 * @author Zhang Senyan
 * Date: 2019-06-27
 *
 * @refitem Ying's blog
 */



#ifndef BELUGA_RPC_SERVER_H
#define BELUGA_RPC_SERVER_H


#include <google/protobuf/service.h>
#include <google/protobuf/stubs/common.h>

#include "beluga/rpc-probuf/beluga.pb.h"
#include "beluga/TCPServer.h"


/**
 * 每个服务对应的信息块
 */
struct ServiceInfo{

    ::google::protobuf::Service* service;

    const ::google::protobuf::ServiceDescriptor* sd;

    //服务注册的方法
    std::map<std::string, const ::google::protobuf::MethodDescriptor*> methods;

};

namespace beluga {

    /**
     * 控制器
     */
    class RpcController : public google::protobuf::RpcController {
        virtual void Reset() {};

        virtual bool Failed() const { return false; };

        virtual std::string ErrorText() const { return "error!"; };

        virtual void StartCancel() {};

        virtual void SetFailed(const std::string &reason) {};

        virtual bool IsCanceled() const { return false; };

        virtual void NotifyOnCancel(google::protobuf::Closure *callback) {};
    };

}

/**
 * RPC 服务器
 */
class RPC_Server {

public:
    //构造函数
    RPC_Server(int port);

    //消息回调
    void onMessage(beluga::CellTaskPtr cellTask);

    //结果回调
    void onResult(::google::protobuf::Message* responseMsg,beluga::CellTaskPtr cellTask);

    void onConnection(const beluga::ConnPtr &connPtr);

    //定时器回调
    void onTimer();

    //启动RPC服务
    void startLoop();

    bool isRunning();

    //注册服务
    void registerService(::google::protobuf::Service* service);

private:

    TCPServer _tcpServer;

    std::map<std::string, ServiceInfo> _services;
};


#endif //BELUGA_RPC_SERVER_H
