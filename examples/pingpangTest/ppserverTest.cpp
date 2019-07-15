
#include <iostream>
#include <functional>

#include "beluga/TCPServer.h"
#include "beluga/log/LogStream.h"
#include "beluga/task/CellTask.h"

using std::placeholders::_1;
using namespace beluga;

class PPServer{
public:

    explicit PPServer(int port):_server(port){
        _server.setMessageCallBack(std::bind(&PPServer::onMessage,this,_1));
        _server.setConnectionCallBack(std::bind(&PPServer::onConnection,this,_1));
        _server.setTimerCallBack(std::bind(&PPServer::onTimer,this));
    }
    void onMessage(CellTaskPtr cellTask){

        //消息处理函数
        //LOG_INFO<<cellTask->getConnAddress()<<" ping-pang:"<<cellTask->getTaskString()<<LOG_ENDL;
        std::string str=cellTask->getTaskString();
        _count+=str.length();
        cellTask->respond(str);
    }
    void onConnection(const ConnPtr &connPtr){

        //接收一个新连接
        //LOG_INFO<<"Accept a new Connection: IP="<<connPtr->getIP()<<" PORT="<<connPtr->getPort()<<LOG_ENDL;
    }
    void onTimer(){
        std::cout<<_count<<std::endl;
        _count=0;
    }
    void startLoop(){

        //启动服务器监听客户端连接
        _server.startListen();
    }
    bool isRunning(){
        return _server.isRunning();
    }
private:
    TCPServer _server;
    long _count;
};

using namespace std;
int main() {

    LOG.resetPath("/home/zhsy/work/ClionWorkspace/beluga/LogTest.log");

    LOG_INFO<<"start Loop"<<LOG_ENDL;
    PPServer ppServer(10000);
    ppServer.startLoop();
    return 0;

}