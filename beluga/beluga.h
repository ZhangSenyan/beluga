//
// Created by zhsy on 19-7-13.
//

#ifndef BELUGA_BELUGA_H
#define BELUGA_BELUGA_H

#include <memory>

class Connection;
class CellTask;
class TaskQueue;

namespace beluga{
    typedef std::shared_ptr<Connection> ConnPtr;
    typedef std::function<void(const ConnPtr&)> ConnFunctor;
    typedef std::shared_ptr<CellTask> PtrCellTask;
    typedef std::shared_ptr<char> PtrChar;
    typedef std::shared_ptr<Connection> ConnPtr;
    typedef std::shared_ptr<TaskQueue> TaskQueuePtr;
    typedef std::shared_ptr<CellTask> CellTaskPtr;
    typedef std::function<void(CellTaskPtr)> WorkFunctor;
    
    
    using std::placeholders::_1;
    using std::placeholders::_2;
    using std::placeholders::_3;
}




#endif //BELUGA_BELUGA_H
