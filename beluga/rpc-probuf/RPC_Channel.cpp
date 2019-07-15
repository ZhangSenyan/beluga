//
// Created by zhsy on 19-7-15.
//

#include "RPC_Channel.h"
#include "beluga/thread/EventLoop.h"
#include "beluga/rpc-probuf/beluga.pb.h"
RPC_Channel::RPC_Channel(char * ip, int port,EventLoop* eventLoop):
        _tcpClient(ip,port),
        _channel(new Channel(0)),
        _eventLoop(eventLoop){

    _channel->setFD(_tcpClient.getFD());

    _channel->setReadHandler(std::bind(&RPC_Channel::handle_read,this));
    _channel->setWriteHandler(std::bind(&RPC_Channel::handle_write,this));
    _channel->setErrorHandler(std::bind(&RPC_Channel::handle_error,this));

    _channel->setEvents( EPOLLET| EPOLLERR);

    eventLoop->addChannel(_channel);
    _requesting= false;

}
void RPC_Channel::handle_read(){
    //std::cout<<"handle_read()"<<std::endl;
    auto retV=_tcpClient.readStream();
    assert(retV.size()==1);
    _recvMsg=retV[0];
    _requesting=false;
    _cv.notify_all();
}
void RPC_Channel::handle_write(){
    //std::cout<<"handle_write()"<<std::endl;
    _tcpClient.write(_sendMsg);
    _channel->removeEvents(EPOLLOUT);
    _channel->addEvents(EPOLLIN);
    _eventLoop->getEpoll()->updateChannel(_channel);
}
void RPC_Channel::handle_error(){
    std::cout<<"handle_error()"<<std::endl;
}
void RPC_Channel::write(const std::string& str){
    //不支持异步调用
    assert(!_requesting);
    _requesting=true;
    _sendMsg=str;
    _channel->addEvents(EPOLLOUT);
    _eventLoop->getEpoll()->updateChannel(_channel);

}
std::string& RPC_Channel::read(){
    assert(_requesting);

    std::unique_lock<std::mutex> l(_mutex);

    while(_requesting){
        _cv.wait(l);
    }
    return _recvMsg;
}
void RPC_Channel::CallMethod(const ::google::protobuf::MethodDescriptor* method,
                        ::google::protobuf::RpcController* ,
                        const ::google::protobuf::Message* request,
                        ::google::protobuf::Message* response,
                        ::google::protobuf::Closure*) {

    std::string serialzied_data = request->SerializeAsString();

    beluga::RPCMsg rpc_msg;
    rpc_msg.set_service_name(method->service()->name());
    rpc_msg.set_method_name(method->name());
    rpc_msg.set_request_data(serialzied_data);

    write(rpc_msg.SerializeAsString());
    response->ParseFromString(read());
}