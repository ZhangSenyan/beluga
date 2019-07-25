/**
 * @author Zhang Senyan
 * Date: 2019-05-09
 *
 * TCP Server
 */

#ifndef BELUGA_TCPSERVER_H
#define BELUGA_TCPSERVER_H

#include <functional>
#include <memory>
#include <set>

#include "beluga.h"
#include "beluga/thread/AcceptThreadPool.h"
#include "beluga/thread/DealThreadPool.h"
#include "beluga/thread/DealThread.h"

class TaskQueue;
class CellTask;

class TCPServer {
public:

    /**
     * 构造函数
     * @param port ： 服务器端口号
     */
    TCPServer(int port);
    ~TCPServer();

    //初始化完成，开始监听
    void startListen();

    void quit();

    //用户回调接口
    void setMessageCallBack(beluga::WorkFunctor onMessage);
    void setConnectionCallBack(beluga::ConnFunctor connFunctor);
    void setDropConnectionCallBack(beluga::ConnFunctor connFunctor);
    void setTimerCallBack(beluga::CallFunc func);

    //get new connections
    void onAccept();

    bool isRunning(){
        return _running;
    }

private:
    int _listenFd;
    bool _running;

    //IO线程池
    AcceptThreadPool _acceptThreads;

    //计算线程池
    DealThreadPool _dealThreads;

    //任务队列
    std::shared_ptr<TaskQueue> _taskQueue;

    beluga::ConnFunctor _onConnection;

    //循环体
    EventLoop _eventLoop;

    beluga::ChannelPtr _listenChannel;
};


#endif //BELUGA_TCPSERVER_H
