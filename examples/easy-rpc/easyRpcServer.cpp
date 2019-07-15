//
// Created by zhsy on 19-7-15.
//


#include "protobuf/echo.pb.h"
#include "beluga/rpc-probuf/RPC_Server.h"
#include "beluga/log/LogStream.h"
class MySumService : public belugaEcho::EchoService {
public:
    virtual void Echo(::google::protobuf::RpcController* /* controller */,
                      const ::belugaEcho::Request* request,
                      ::belugaEcho::Response* response,
                      ::google::protobuf::Closure* done) {
        response->set_sum(request->a()+request->b());
        done->Run();
    }
};

int main() {
    LOG.resetPath("/media/zhsy/Document/beluga/LogTest.log");
    RPC_Server rpc_server(10000);

    MySumService sum_service;
    rpc_server.registerService(&sum_service);
    rpc_server.startLoop();

    return 0;
}
