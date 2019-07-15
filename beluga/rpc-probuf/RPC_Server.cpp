//
// Created by zhsy on 19-7-15.
//
//cpp



#include "RPC_Server.h"
#include "beluga/log/LogStream.h"

using namespace beluga;
RPC_Server::RPC_Server(int port):_tcpServer(port){

    _tcpServer.setMessageCallBack(std::bind(&RPC_Server::onMessage,this,_1));
    _tcpServer.setConnectionCallBack(std::bind(&RPC_Server::onConnection,this,_1));
    _tcpServer.setTimerCallBack(std::bind(&RPC_Server::onTimer,this));
}
void RPC_Server::onMessage(CellTaskPtr cellTask){

    //消息处理函数
    LOG_INFO<<cellTask->getConnAddress()<<" ping-pang:"<<cellTask->getTaskString()<<LOG_ENDL;

    beluga::RPCMsg rpcMsg;
    //解析收到的字符串
    rpcMsg.ParseFromString(cellTask->getTaskString());

    //解析方法、名称、数据
    std::string service_name=rpcMsg.service_name();
    std::string method_name=rpcMsg.method_name();
    std::string request_data=rpcMsg.request_data();


    auto service=_services[service_name].service;
    auto method=_services[service_name].methods[method_name];


    auto requestMsg = service->GetRequestPrototype(method).New();
    requestMsg->ParseFromString(request_data);
    auto responseMsg = service->GetResponsePrototype(method).New();

    beluga::RpcController controller;
    int fd;
    auto done = ::google::protobuf::NewCallback(
            this,
            &RPC_Server::onResult,
            responseMsg,
            cellTask);
    service->CallMethod(method, &controller, requestMsg, responseMsg, done);


}
void RPC_Server::onResult(::google::protobuf::Message* responseMsg,
              CellTaskPtr cellTask){
    cellTask->respond(responseMsg->SerializeAsString());
}
void RPC_Server::onConnection(const ConnPtr &connPtr){

    //接收一个新连接
    LOG_INFO<<"Accept a new Connection: IP="<<connPtr->getIP()<<" PORT="<<connPtr->getPort()<<LOG_ENDL;
}
void RPC_Server::onTimer(){

}
void RPC_Server::startLoop(){

    _tcpServer.startListen();
}
bool RPC_Server::isRunning(){
    return _tcpServer.isRunning();
}

void RPC_Server::registerService(::google::protobuf::Service* service) {
    ServiceInfo service_info;
    service_info.service = service;
    service_info.sd = service->GetDescriptor();

    for (int i = 0; i < service_info.sd->method_count(); ++i) {
        service_info.methods[service_info.sd->method(i)->name()] = service_info.sd->method(i);
    }
    _services[service_info.sd->name()] = service_info;
}