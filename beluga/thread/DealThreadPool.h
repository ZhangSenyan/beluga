/**
 * @author Zhang Senyan
 * Date: 2019-04-29
 *
 * 计算线程池
 */

#ifndef HCCSERVER_DEALTHREADPOOL_H
#define HCCSERVER_DEALTHREADPOOL_H

#include <vector>
#include "DealThread.h"

class DealThreadPool {
public:

    /**
     * 构造函数
     * @param size ： 计算线程数量
     */
    DealThreadPool(int size);
    ~DealThreadPool();

    void setTaskQueue(std::shared_ptr<TaskQueue> taskQueue);

    //启动任务循环
    void startLoop();

    //用户注册消息回显
    void setMessageCallBack(beluga::WorkFunctor workFunctor);

private:
    std::vector<std::shared_ptr<DealThread>> _threads;
};

#endif //HCCSERVER_DEALTHREADPOOL_H
