/**
 * @author Zhang Senyan
 * Date: 2019-04-29
 *
 * 计算线程
 */

#ifndef BELUGA_DEALTHREAD_H
#define BELUGA_DEALTHREAD_H

#include <memory>
#include <thread>

#include "beluga/beluga.h"


class TaskQueue;
class CellTask;

class DealThread {
public:

    DealThread();
    ~DealThread();

    //启动处理线程
    void startLoop();

    //计算任务循环
    void HandleLoop();

    void setTaskQueue(beluga::TaskQueuePtr taskQueue);

    //用户注册消息回调函数
    void setMessageCallBack(beluga::WorkFunctor workFunctor);

private:
    //状态位
    bool _runing;
    std::thread _t;
    std::shared_ptr<TaskQueue> _taskQueue;

    //消息处理函数
    beluga::WorkFunctor _workFunctor;
};

#endif //BELUGA_DEALTHREAD_H
