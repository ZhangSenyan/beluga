
#include <iostream>
#include <functional>

#include "beluga/TCPServer.h"
#include "beluga/log/LogStream.h"
#include "beluga/task/CellTask.h"

using namespace std::placeholders;


class EchoServer{
public:
    typedef std::shared_ptr<CellTask> CellTaskPtr;
    explicit EchoServer(int port):_server(port){
        _server.setMessageCallBack(std::bind(&EchoServer::onMessage,this,_1));
        _server.setConnectionCallBack(std::bind(&EchoServer::onConnection,this,_1));
    }
    void onMessage(CellTaskPtr cellTask){

        //消息处理函数
        LOG_INFO<<cellTask->getConnAddress()<<" echo:"<<cellTask->getTaskString()<<LOG_ENDL;

        cellTask->respond(cellTask->getTaskString());
    }
    void onConnection(TCPServer::ConnPtr connPtr){

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