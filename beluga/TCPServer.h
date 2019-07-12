//
// Created by zhsy on 19-6-8.
//

#ifndef HCCSERVER_SERVER_H
#define HCCSERVER_SERVER_H

#include <functional>
#include <memory>
#include <set>
#include "beluga/thread/AcceptThreadPool.h"
#include "beluga/thread/DealThreadPool.h"

class TaskQueue;
class CellTask;
#include "beluga/thread/DealThread.h"
class TCPServer {
public:
    typedef std::shared_ptr<CellTask> PtrCellTask;
    typedef std::shared_ptr<char> PtrChar;
    typedef std::shared_ptr<Connection> ConnPtr;
    typedef std::function<void(const ConnPtr conn)> ConnFunctor;
    TCPServer(int port);
    ~TCPServer();
    void startListen();
    void quit();
    void setMessageCallBack(DealThread::WorkFunctor onMessage);
    void setConnectionCallBack(ConnFunctor connFunctor);
private:
    int _listenFd;
    bool _running;
    AcceptThreadPool _acceptThreads;
    DealThreadPool _dealThreads;
    std::shared_ptr<TaskQueue> _taskQueue;
    ConnFunctor _onConnection;
};


#endif
