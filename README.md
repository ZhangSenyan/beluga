# beluga (a simple C++ network library) 
[![build status](https://travis-ci.org/ZhangSenyan/beluga.svg?branch=master)](https://travis-ci.org/ZhangSenyan/beluga)
[![license](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-Linux-green.svg?style=flat)](https://github.com/SenyanZhang/beluga)


# Introduction
beluga是一款小型的C++网络库，支持高并发，多线程，用于实现高性能网络服务。
beluga 基于事件驱动，采用非阻塞IO，整体框架由mainReactor,SubReactor,Worker Threads Pool三部分组成。

# Technical points
* 基于Reactor模式，包含监听线程，IO线程和计算线程，监听线程负责监听和创建连接，IO线程负责读取写入网络数据，
计算线程负责具体业务逻辑
* IO线程采用EPOLL边沿触发多路复用技术，非阻塞IO
* 主线程接收创建连接后以Least Connections方式，分发给IO线程
* 采用双任务队列机制，实现紧急任务优先处理
* 基于时间轮机制和哈希表实现心跳检测，剔除超时连接
* 为了避免内存泄漏和野指针，重复delete等问题，对于线程间对象使用智能指针等RALL机制
* 支持定时定量发送数据
* 采用异步日志系统，基于buffer对象池实现日志快速写入
* 内存池采用动态倍增扩容机制以及lazy加载模式，最大限度节省内存开销，提高内存申请速率
* 采用多线程和线程池，充分利用多核CPU，减少线程频繁创建带来的开销


# Design Guidance

>#### [网络模型](https://github.com/ZhangSenyan/beluga/blob/master/设计思路.md#网络模型)
>#### [内存池设计](https://github.com/ZhangSenyan/beluga/blob/master/设计思路.md#内存池设计)
>#### [Log日志系统](https://github.com/ZhangSenyan/beluga/blob/master/设计思路.md#Log日志系统)
>#### [双任务队列](https://github.com/ZhangSenyan/beluga/blob/master/设计思路.md#双任务队列)
>#### [心跳检测](https://github.com/ZhangSenyan/beluga/blob/master/设计思路.md#心跳检测)
>#### [缓冲区Buffer设计](https://github.com/ZhangSenyan/beluga/blob/master/设计思路.md#缓冲区Buffer设计)


# Build
 
```cpp
git clone --recursive https://github.com/SenyanZhang/beluga.git
cd beluga
mkdir build
cd build
cmake ..
make
```
# Configure


# Examples
## An echo TCP server
```cpp
#include <iostream>
#include <functional>

#include "beluga/TCPServer.h"
#include "beluga/log/LogStream.h"
#include "beluga/task/CellTask.h"

using std::placeholders::_1;
using namespace beluga;

class EchoServer{
public:

    explicit EchoServer(int port):_server(port){
        _server.setMessageCallBack(std::bind(&EchoServer::onMessage,this,_1));
        _server.setConnectionCallBack(std::bind(&EchoServer::onConnection,this,_1));

    }
    void onMessage(CellTaskPtr cellTask){

        //消息处理函数
        LOG_INFO<<cellTask->getConnAddress()<<" echo:"<<cellTask->getTaskString()<<LOG_ENDL;

        cellTask->respond(cellTask->getTaskString());
    }
    void onConnection(const ConnPtr &connPtr){

        //接收一个新连接
        LOG_INFO<<"Accept a new Connection: IP="<<connPtr->getIP()<<" PORT="<<connPtr->getPort()<<LOG_ENDL;
    }
    void startLoop(){

        //启动服务器监听客户端连接
        _server.startListen();
    }

private:
    TCPServer _server;
};

using namespace std;
int main() {

    LOG.resetPath("/home/zhsy/work/ClionWorkspace/beluga/LogTest.log");

    LOG_INFO<<"start Loop"<<LOG_ENDL;
    EchoServer echoServer(9000);
    echoServer.startLoop();
    return 0;
}
```

## An easy RPC server
```cpp

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
```

## A chat TCP server

```cpp
#include <set>

#include "beluga/TCPServer.h"
#include "beluga/beluga.h"
#include "beluga/log/LogStream.h"

using namespace beluga;

class ChatServer{
public:
    explicit ChatServer(int port):_server(port){
        _server.setMessageCallBack(std::bind(&ChatServer::onMessage,this,_1));
        _server.setConnectionCallBack(std::bind(&ChatServer::onConnection,this,_1));
        _server.setDropConnectionCallBack(std::bind(&ChatServer::onDropConnection,this,_1));
    }
    void onMessage(CellTaskPtr cellTask){

        //消息处理函数
        LOG_INFO<<cellTask->getConnAddress()<<" echo:"<<cellTask->getTaskString()<<LOG_ENDL;

        std::string msg=cellTask->getTaskString();
        ConnPtr connPtr=cellTask->getConn();

        for(auto &conn:connSet){
            if(conn!=connPtr){
                conn->writeBuffer(msg);
            }
        }

    }
    void onConnection(const ConnPtr &connPtr){

        //接收一个新连接
        LOG_INFO<<"Accept a new Connection: IP="<<connPtr->getIP()<<" PORT="<<connPtr->getPort()<<LOG_ENDL;

        connSet.insert(connPtr);


    }
    void onDropConnection(const ConnPtr &connPtr){

        //注销一个连接
        LOG_INFO<<"Drop Connection: IP="<<connPtr->getIP()<<" PORT="<<connPtr->getPort()<<LOG_ENDL;
        connSet.erase(connPtr);
    }
    void startLoop(){

        //启动服务器监听客户端连接
        _server.startListen();
    }

private:
    TCPServer _server;
    std::set<ConnPtr> connSet;
};

int main(){
    LOG.resetPath("/home/zhsy/work/ClionWorkspace/beluga/LogTest.log");

    LOG_INFO<<"start Loop"<<LOG_ENDL;
    ChatServer chatServer(9000);
    chatServer.startLoop();
    return 0;

}
```
## More examples
[beluga/examples](https://github.com/ZhangSenyan/beluga/tree/master/examples)
# Performance Test
### 吞吐量测试
##### 单线程

| 连接数      | 1           | 10          | 100         | 1000        | 10000       |
| :--------: | :---------: | :---------: | :---------: | :---------: | :---------: |
| Throughput(MiB/s)| 396    | 1109       | 1137        | 818          | 695          |
* IO线程数量：1     
* 计算线程数量：1    
* 客户端线程数：1 
##### 2线程 
| 连接数      | 1           | 10          | 100         | 1000        | 10000       |
| :--------: | :---------: | :---------: | :---------: | :---------: | :---------: |
| Throughput(MiB/s)| 756    | 2105          | 1232     | 929          | 890          |
* IO线程数量：2     
* 计算线程数量：2    
* 客户端线程数：2
##### 4线程
| 连接数      | 1           | 10          | 100         | 1000        | 10000       |
| :--------: | :---------: | :---------: | :---------: | :---------: | :---------: |
| Throughput(MiB/s)| 1265    | 2878       | 1697       | 1105          | 910          |
* IO线程数量：4     
* 计算线程数量：4    
* 客户端线程数：4 

### 日志系统测试

### 测试环境
* 操作系统：Linux Ubuntu 16.04
* 硬件CPU：Intel(R) Core(TM) i7-8700 CPU @ 3.20GHz  12核
* 内存：Kinston DDR4 2400 16GB

# Reference
> * #### Linux多线程服务端编程 - 陈硕
> * #### Unix环境编程
> * #### 高性能服务器编程
> * #### Ying's blog (RPC)
> * #### Jim Fawcett, CSE687 
> * #### C++ primer