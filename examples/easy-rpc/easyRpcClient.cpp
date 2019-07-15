//
// Created by zhsy on 19-7-15.
//
#include "beluga/rpc-probuf/RPC_Channel.h"
#include "protobuf/echo.pb.h"
#include "beluga/rpc-probuf/RPC_Server.h"
int main(){
    EventLoop eventLoop;
    RPC_Channel rpcChannel("10.20.4.5",10000,&eventLoop);
    eventLoop.startLoop();

    belugaEcho::Request request;
    belugaEcho::Response response;
    request.set_a(15);
    request.set_b(10);

    belugaEcho::EchoService_Stub stub(&rpcChannel);
    beluga::RpcController cntl;
    stub.Echo(&cntl, &request, &response, NULL);
    std::cout << "sum:" << response.sum() << std::endl;
}