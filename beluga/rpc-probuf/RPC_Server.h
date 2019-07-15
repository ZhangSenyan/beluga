//
// Created by zhsy on 19-7-15.
// ref: Ying's Blog
//

#ifndef BELUGA_RPC_SERVER_H
#define BELUGA_RPC_SERVER_H


#include <google/protobuf/service.h>
#include <google/protobuf/stubs/common.h>

#include "beluga/rpc-probuf/beluga.pb.h"
#include "beluga/TCPServer.h"



struct ServiceInfo{
    ::google::protobuf::Service* service;
    const ::google::protobuf::ServiceDescriptor* sd;
    std::map<std::string, const ::google::protobuf::MethodDescriptor*> methods;
};
namespace beluga {
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
class RPC_Server {
public:
    RPC_Server(int port);
    void onMessage(beluga::CellTaskPtr cellTask);
    void onResult(::google::protobuf::Message* responseMsg,beluga::CellTaskPtr cellTask);
    void onConnection(const beluga::ConnPtr &connPtr);
    void onTimer();
    void startLoop();
    bool isRunning();

    void registerService(::google::protobuf::Service* service);

private:
    TCPServer _tcpServer;
    std::map<std::string, ServiceInfo> _services;
};


#endif //BELUGA_RPC_SERVER_H
