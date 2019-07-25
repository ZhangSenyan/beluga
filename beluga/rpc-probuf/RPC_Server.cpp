/**
 * @author Zhang Senyan
 * Date: 2019-06-27
 *
 */

#include "RPC_Server.h"
//#include "beluga/log/LogStream.h"

using namespace beluga;

RPC_Server::RPC_Server(int port):
    _tcpServer(port){

    //设置回调函数
    _tcpServer.setMessageCallBack(std::bind(&RPC_Server::onMessage,this,_1));
    _tcpServer.setConnectionCallBack(std::bind(&RPC_Server::onConnection,this,_1));
    _tcpServer.setTimerCallBack(std::bind(&RPC_Server::onTimer,this));

}

/**
 * tcp_server 消息回调函数
 * @param cellTask 收到的任务消息
 */
void RPC_Server::onMessage(CellTaskPtr cellTask){

    //消息处理函数
    beluga::RPCMsg rpcMsg;

    //解析收到的字符串
    rpcMsg.ParseFromString(cellTask->getTaskString());

    /**
     * 解析请求消息
     * ---------------------------
     * 1.service_name 服务
     * 2.method_name 方法
     * 3.request_data  请求的数据
     * ---------------------------
     */
    std::string service_name=rpcMsg.service_name();
    std::string method_name=rpcMsg.method_name();
    std::string request_data=rpcMsg.request_data();

    //找到对应的服务和方法
    auto service=_services[service_name].service;
    auto method=_services[service_name].methods[method_name];

    //解析请求数据
    auto requestMsg = service->GetRequestPrototype(method).New();
    requestMsg->ParseFromString(request_data);
    auto responseMsg = service->GetResponsePrototype(method).New();

    //调用相应方法，注册回调函数
    beluga::RpcController controller;
    int fd;
    auto done = ::google::protobuf::NewCallback(
            this,
            &RPC_Server::onResult,
            responseMsg,
            cellTask);
    service->CallMethod(method, &controller, requestMsg, responseMsg, done);

}

/**
 * 服务响应处理函数
 * 当调用服务方法处理信息后，调用该函数
 * ------------------------------
 * @param responseMsg ： 需要回复给客户端的消息
 * @param cellTask
 */
void RPC_Server::onResult(::google::protobuf::Message* responseMsg,
              CellTaskPtr cellTask){
    cellTask->respond(responseMsg->SerializeAsString());
}

void RPC_Server::onConnection(const ConnPtr &connPtr){

    //接收一个新连接
    //LOG_INFO<<"Accept a new Connection: IP="<<connPtr->getIP()<<" PORT="<<connPtr->getPort()<<LOG_ENDL;
}

void RPC_Server::onTimer(){

}

void RPC_Server::startLoop(){

    _tcpServer.startListen();
}

bool RPC_Server::isRunning(){
    return _tcpServer.isRunning();
}

/**
 * 向RPC服务器中注册一个服务
 * @param service
 */
void RPC_Server::registerService(::google::protobuf::Service* service) {

    //填写服务信息
    ServiceInfo service_info;
    service_info.service = service;
    service_info.sd = service->GetDescriptor();

    //注册服务方法
    for (int i = 0; i < service_info.sd->method_count(); ++i) {
        service_info.methods[service_info.sd->method(i)->name()] = service_info.sd->method(i);
    }

    //注册服务
    _services[service_info.sd->name()] = service_info;
}