/**
 * @author Zhang Senyan
 * Date: 2019-04-20
 *
 * SubReactor: IO Thread
 */

#ifndef BELUGA_ACCEPTTHREAD_H
#define BELUGA_ACCEPTTHREAD_H


#include <iostream>
#include <map>

#include "beluga/beluga.h"
#include "beluga/net/Connection.h"
#include "beluga/task/TaskQueue.h"
#include "beluga/base/TimingWheel.h"
#include "beluga/thread/EventLoop.h"



class AcceptThread :public EventLoop{
public:

    /**
     * 构造函数
     * @param size ： 本线程允许的最大连接数
     * @param timer_ms  ： 定时周期
     *                    用于定时定量发送数据
     */
    AcceptThread(int size=10000,int timer_ms=50);
    ~AcceptThread();

    //添加或移除连接
    void addConnction(std::shared_ptr<Connection> conn);
    void removeConnction(std::shared_ptr<Connection> conn);

    /**
     * 移除并关闭客户端
     * @param fd ：客户端对应的文件描述符
     */
    void removeConnction(int fd);


    //定时处理函数
    void timerHandle() override ;

    //50ms 定时任务
    void timerHandle50();

    //1s 定时任务
    void timerHandle1000();

    //设定任务队列入口
    void setTaskQueue(const std::shared_ptr<TaskQueue>& taskQueue);
    std::shared_ptr<TaskQueue> getTaskQueue();

    std::map<int ,std::shared_ptr<Connection>>& getConnMap() ;

    /**
     * @return 该 IO 线程所负责的 Connection 数量
     */
    int getConnSize() const;

    int updateConn(Connection::ConnPtr conn);

    //掉线回调
    void setDropConnectionCallBack(beluga::ConnFunctor dropFunctor);
private:

    /**
     * map<Connection fd,  Connection ptr>
     * 注册 文件描述符 和 对应的Connection
     */
    std::map<int,std::shared_ptr<Connection>> connMap;

    //任务队列，工作在IO线程与计算线程之间
    std::shared_ptr<TaskQueue> _taskQueue;

    //时间轮，用于心跳检测
    TimingWheel _timingWheel;

    int _timeCount;

    //掉线回调函数
    beluga::ConnFunctor _onDropConntion;
    
};


#endif //BELUGA_ACCEPTTHREAD_H
