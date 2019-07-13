//
// Created by zhsy on 19-6-8.
//

#ifndef HCCSERVER_SERVER_H
#define HCCSERVER_SERVER_H

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
    TCPServer(int port);
    ~TCPServer();
    void startListen();
    void quit();
    void setMessageCallBack(beluga::WorkFunctor onMessage);
    void setConnectionCallBack(beluga::ConnFunctor connFunctor);
    void setDropConnectionCallBack(beluga::ConnFunctor connFunctor);
private:
    int _listenFd;
    bool _running;
    AcceptThreadPool _acceptThreads;
    DealThreadPool _dealThreads;
    std::shared_ptr<TaskQueue> _taskQueue;
    beluga::ConnFunctor _onConnection;
};


#endif
