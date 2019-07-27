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

        //接收一个新连接
        LOG_INFO<<"Drop Connection: IP="<<connPtr->getIP()<<" PORT="<<connPtr->getPort()<<LOG_ENDL;
        connSet.erase(connPtr);
    }
    void startLoop(){

        //启动服务器监听客户端连接
        _server.startListen();
    }

private:
    TCPServer _server;

    //已重载operator<(ConnPtr,ConnPtr)
    //是安全的
    std::set<ConnPtr> connSet;
};

int main(){
    LOG.resetPath("/home/zhsy/work/ClionWorkspace/beluga/LogTest.log");

    LOG_INFO<<"start Loop"<<LOG_ENDL;
    ChatServer chatServer(9000);
    chatServer.startLoop();
    return 0;

}