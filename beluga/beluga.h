/**
 * @author Zhang Senyan
 * Date: 2019-05-10
 *
 */

#ifndef BELUGA_BELUGA_H
#define BELUGA_BELUGA_H

#include <memory>

class Connection;
class CellTask;
class TaskQueue;
class Channel;
class Basket;

namespace beluga{

    typedef std::shared_ptr<Connection> ConnPtr;

    typedef std::function<void(const ConnPtr&)> ConnFunctor;

    typedef std::shared_ptr<CellTask> PtrCellTask;

    typedef std::shared_ptr<char> PtrChar;

    typedef std::shared_ptr<Connection> ConnPtr;

    typedef std::shared_ptr<TaskQueue> TaskQueuePtr;

    typedef std::shared_ptr<CellTask> CellTaskPtr;

    typedef std::shared_ptr<Basket> BasketPtr;

    typedef std::function<void(CellTaskPtr)> WorkFunctor;

    typedef std::shared_ptr<Channel> ChannelPtr;

    typedef std::function<void(void)> CallFunc;


    using std::placeholders::_1;
    using std::placeholders::_2;
    using std::placeholders::_3;


    enum class LoopMethod{
        detach,
        join
    };


}




#endif //BELUGA_BELUGA_H
