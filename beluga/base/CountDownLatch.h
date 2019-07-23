/**
 * @author Zhang Senyan
 * Date: 2019-06-10
 *
 * 同步屏障
 */


#ifndef BELUGA_COUNTDOWNLATCH_H
#define BELUGA_COUNTDOWNLATCH_H

#include <mutex>
#include <condition_variable>

#include "beluga/base/noncopyable.h"

class CountDownLatch : noncopyable
{
public:
    //构造函数
    explicit CountDownLatch(int count);


    void countDown();

    //获取计数值
    int getCount() const;

private:
    //锁
    mutable std::mutex _mutex;

    //条件变量
    std::condition_variable _condition;

    //计数值
    int _count;
};


#endif  // BELUGA_COUNTDOWNLATCH_H
