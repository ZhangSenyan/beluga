/**
 * @author Zhang Senyan
 * Date: 2019-06-25
 *
 * 异步日志写入过程
 *
 * 工作线程 -> {|logBuffer1|--|logBuffer2|- ... -|logBufferN|} -> 日志写入线程 -> 写入文件
 *                  |                                |
 *              取 Buffer <==== |logBuffer| <====归还Buffer
 *                              |logBuffer|
 *                       对象池      ...
 *                              |logBuffer|
 *
 * 日志写入时判断当前 LOG-BUFFER 是否写满，如果写满就将 LOG-BUFFER 压入队列，并从对象池中取一个新的LOG-BUFFER
 * 压入队列后，写入线程开始写入日志，写入完成后，将LOG-BUFFER归还给对象池
 *
 * 如果LOG-BUFFER超过一定时间都没有写满，那么写入线程将自动将其压入队列，执行写入操作
 *
 */

#ifndef BELUGA_ASYNLOGGING_H
#define BELUGA_ASYNLOGGING_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <algorithm>

#include "beluga/log/LogFile.h"
#include "beluga/log/LogBuffer.h"
#include "beluga/log/ObjectPool.h"


class AsynLogging {
public:

    //所使用LogBuffer大小为4096
    typedef LogBuffer<4096> BigBuffer;

    //构造函数
    AsynLogging();
    ~AsynLogging();

    void setLogPath(std::string logPath);
    void startLoop();

    //写入日志-字符串
    bool append(const char *ch,size_t len);

    //写入日志-数值
    template <typename T>
    bool append(T v);

    //日志文件主循环
    void HandleLoop();

    /**
     * 如果超过一定时间都没有写满日志，
     * 会自动刷新当前缓冲区，将其写入文件
     */
    void curBufferFlush();

private:

    //日志队列
    std::queue<BigBuffer*> _buffQueue;

    //对外日志缓冲区
    BigBuffer* _curBuffer;

    //正在写入磁盘文件的日志缓冲区
    BigBuffer* _writingBuffer;

    //对象池
    ObjectPool<BigBuffer> _objectPool;

    //队列锁
    std::mutex _mutexQueue;

    //LOG锁
    std::mutex _mutexLog;

    std::condition_variable _cv;

    //日志队列的最大容量，一定程度限流
    int _maxSize;

    //日志进程状态位
    bool _runing;

    //日志进程
    std::thread _t;

    //日志文件
    LogFile _logFile;

    bool update;
};

/**
 * 写入数值
 * @tparam T 数值类型
 * @param v 值
 * @return  true ： 写入成功
 *          false ： 写入失败(当队列容量超过限定体积时)
 */
template <typename T>
bool AsynLogging::append(T v){
    std::unique_lock<std::mutex> locklog(_mutexLog);

    if(-1==_curBuffer->append(v)){
        //当前缓冲区满
        {
            std::unique_lock<std::mutex> lockqueue(_mutexQueue);

            //检测日志队列是否超过限定长度
            if (_buffQueue.size() > _maxSize)
                return false;

            //将当期缓冲区压入队列
            _buffQueue.push(_curBuffer);
        }

        //通知日志线程开始写入文件
        _cv.notify_all();

        //向对象池申请一个新的LogBuffer
        _curBuffer=_objectPool.alloc();

        //将数据写入新的LogBuffer
        _curBuffer->append(v);
        //assert(-1!=_curBuffer->append(v));
    }
    return true;
}

#endif //BELUGA_ASYNLOGGING_H
