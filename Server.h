//
// Created by zhsy on 19-6-8.
//

#ifndef HCCSERVER_SERVER_H
#define HCCSERVER_SERVER_H

#include <functional>
#include <memory>
#include "task/CellTask.h"
#include "net/Epoll.h"
#include "thread/AcceptThread.h"
#include <set>

class Server {
public:
    typedef std::shared_ptr<CellTask> PtrCellTask;
    typedef std::shared_ptr<char> PtrChar;
    typedef std::function<int(PtrCellTask,PtrChar)> Dealer;
    Server(int port);
    ~Server();
    int registerDealer(Dealer dealer);
    void startListen();
    void quit();
private:
    int _listenFd;
    bool _running;
    AcceptThread _acceptThread;
    std::set<std::shared_ptr<Connection> > _conns;

};


#endif
