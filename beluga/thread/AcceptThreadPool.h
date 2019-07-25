/**
 * @author Zhang Senyan
 * Date: 2019-04-25
 *
 */

#ifndef BELUGA_ACCEPTTHREADPOOL_H
#define BELUGA_ACCEPTTHREADPOOL_H

#include <cstddef>
#include <vector>
#include <memory>

#include "beluga/thread/AcceptThread.h"
#include "beluga/net/Connection.h"


class AcceptThreadPool {
public:

    /**
     * 构造函数
     * @param size : 线程数量
     * @param maxConnSize ： 线程池最大连接数量
     */
    AcceptThreadPool(int size,int maxConnSize);

    void setTaskQueue(const std::shared_ptr<TaskQueue>& taskQueue);

    /**
     * 每个连接都是先扔到线程池中
     * 再决定分配到哪一个IO线程
     * ----------------------
     * @param conn ： 连接
     * @return -1：添加失败
     */
    int putConnection(std::shared_ptr<Connection> conn);

    //找到当前负载最小的连接
    std::shared_ptr<AcceptThread> getMinConnThread();

    //启动线程池
    void startLoop();

    //IO线程数量
    int size();

    //连接数上限
    int maxConnSize();

    //当前连接数量
    int connSize();

    void setDropConnectionCallBack(beluga::ConnFunctor dropFunctor);
private:

    //线程池
    std::vector<std::shared_ptr<AcceptThread>> _threads;

    size_t _connNum;
    size_t _maxNum;
};



#endif //BELUGA_ACCEPTTHREADPOOL_H
