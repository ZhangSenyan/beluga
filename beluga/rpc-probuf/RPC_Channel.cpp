/**
 * @author Zhang Senyan
 * Date: 2019-06-27
 *
 * RPC 请求端 channel
 */

#include "RPC_Channel.h"
#include "beluga/thread/EventLoop.h"
#include "beluga/rpc-probuf/beluga.pb.h"

/**
 * 构造函数
 */
RPC_Channel::RPC_Channel(char * ip, int port,EventLoop* eventLoop):
        _tcpClient(ip,port),
        _channel(new Channel(0)),
        _eventLoop(eventLoop){

    //设定channel对应的文件描述符
    _channel->setFD(_tcpClient.getFD());

    //设定读写回调
    _channel->setReadHandler(std::bind(&RPC_Channel::handle_read,this));
    _channel->setWriteHandler(std::bind(&RPC_Channel::handle_write,this));
    _channel->setErrorHandler(std::bind(&RPC_Channel::handle_error,this));

    //边沿触发
    _channel->setEvents( EPOLLET| EPOLLERR);

    eventLoop->addChannel(_channel);
    _requesting= false;

}

/**
 * 读处理函数
 */
void RPC_Channel::handle_read(){

    auto retV=_tcpClient.readStream();
    assert(retV.size()==1);
    _recvMsg=retV[0];
    _requesting=false;
    _cv.notify_all();
}

/**
 * 写处理函数
 */
void RPC_Channel::handle_write(){

    _tcpClient.write(_sendMsg);
    _channel->removeEvents(EPOLLOUT);
    _channel->addEvents(EPOLLIN);
    _eventLoop->getEpoll()->updateChannel(_channel);
}


void RPC_Channel::handle_error(){
    std::cout<<"handle_error()"<<std::endl;
}

/**
 * 将请求写入缓冲，请求发送
 * @param str
 */
void RPC_Channel::write(const std::string& str){

    //不支持异步调用
    assert(!_requesting);
    _requesting=true;

    _sendMsg=str;

    //监听发送事件
    _channel->addEvents(EPOLLOUT);
    _eventLoop->getEpoll()->updateChannel(_channel);

}

/**
 * 以阻塞方式等待响应回复
 * @return 远程服务结果返回
 */
std::string& RPC_Channel::read(){
    assert(_requesting);

    std::unique_lock<std::mutex> l(_mutex);

    while(_requesting){
        _cv.wait(l);
    }
    return _recvMsg;
}

/**
 * rpc call remote method
 * ----------------------------
 * @param method
 * @param request : 请求的数据
 * @param response ：响应的数据
 * ----------------------------
 */
void RPC_Channel::CallMethod(const ::google::protobuf::MethodDescriptor* method,
                        ::google::protobuf::RpcController* ,
                        const ::google::protobuf::Message* request,
                        ::google::protobuf::Message* response,
                        ::google::protobuf::Closure*) {

    //请求序列化
    std::string serialzied_data = request->SerializeAsString();

    /**
     * 填写请求信息
     * 1.服务
     * 2.方法
     * 3.请求数据
     */
    beluga::RPCMsg rpc_msg;
    rpc_msg.set_service_name(method->service()->name());
    rpc_msg.set_method_name(method->name());
    rpc_msg.set_request_data(serialzied_data);

    //发送请求信息
    write(rpc_msg.SerializeAsString());

    //等待接收响应数据
    response->ParseFromString(read());
}