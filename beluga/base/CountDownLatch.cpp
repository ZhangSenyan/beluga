/**
 * @author Zhang Senyan
 * Date: 2019-06-10
 */


#include "beluga/base/CountDownLatch.h"

// 构造函数
CountDownLatch::CountDownLatch(int count)
  : _mutex(),
    _condition(),
    _count(count)
{
}

/**
 * Function： 每来一个线程 count值 减 1
 * 当所有线程都到达时，通知所有线程开始运行
 */
void CountDownLatch::countDown()
{
    std::unique_lock<std::mutex> l(_mutex);

    //count值 减 1
    --_count;
    if (_count == 0)
    {
        //所有线程已经到齐，通知所有线程
        _condition.notify_all();
    }
    else{

        //不满足条件就循环等待
        while (_count > 0)
        {
           _condition.wait(l);
        }

    }
}

// 返回当前到达栅栏的进程数量
int CountDownLatch::getCount() const
{
    std::unique_lock<std::mutex> l(_mutex);
    return _count;
}

