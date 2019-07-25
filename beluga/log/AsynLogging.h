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
    AsynLogging():
        _objectPool(),
        _curBuffer(nullptr),
        _maxSize(100),
        _runing(false),
        _t(std::mem_fun(&AsynLogging::HandleLoop),this),
        _logFile(){

        _curBuffer=_objectPool.alloc();
    }
    ~AsynLogging(){}


    /**
     * 设置log输出文件
     * 转调用logFile相关文件
     * @param logPath ： log文件路径
     */
    void setLogPath(std::string logPath){
        _logFile.resetLogPath(logPath);
    }

    /**
     * 启动日志线程
     */
    void startLoop(){
        _t.detach();
        _runing=true;
    }

    /**
     * 将日志文件写入buffer
     * @param ch 待写入字符串
     * @param len 字符串长度
     * @return false ： 写入失败
     */
    bool append(const char *ch,size_t len){

        std::unique_lock<std::mutex> locklog(_mutexLog);

        if(-1==_curBuffer->append(ch,len)){
            //当前buffer已满，需要更换
            {
                std::unique_lock<std::mutex> lockqueue(_mutexQueue);

                //检测是否日志队列是否超过限制
                if (_buffQueue.size() > _maxSize)
                    return false;

                //将日志压入队列
                _buffQueue.push(_curBuffer);

            }

            //通知日志线程开始写入
            _cv.notify_all();

            //更换日志缓冲
            _curBuffer=_objectPool.alloc();

            //写入日志
            _curBuffer->append(ch,len);
            //assert(-1!=_curBuffer->append(ch,len));
        }
        return true;
    }



    /**
     * 写入数值
     * @tparam T 数值类型
     * @param v 值
     * @return  true ： 写入成功
     *          false ： 写入失败(当队列容量超过限定体积时)
     */
    template <typename T>
    bool append(T v){
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

    /**
     * 日志线程主循环
     * 1. 等待日志队列，有日志到来就立刻写入磁盘
     * 2. 定时检测日志缓冲区，如果一直没有写满，超过时间限制，就自动写入磁盘
     */
    void HandleLoop(){
        /**
         * 待完善
         * 使用countdownlatch
         */
        while (!_runing)
            usleep(1000);

        //启动主循环
        while (_runing){
            {
                //睡在日志队列上，等待LogBuffer的到来
                //或者等待3秒超时
                std::unique_lock<std::mutex> lckQ(_mutexQueue);
                if (_buffQueue.empty()) {
                    _cv.wait_for(lckQ, std::chrono::seconds(3));
                }
            }

            //日志队列为空，说明已超时
            if (_buffQueue.empty()) {

                //将当前LogBuffer压入队列
                curBufferFlush();
            }
            while(!_buffQueue.empty()){

                {
                    //从队列中取出一个LogBuffer
                    std::unique_lock<std::mutex> lckQ(_mutexQueue);
                    _writingBuffer = _buffQueue.front();
                    _buffQueue.pop();
                }

                assert(_writingBuffer!= nullptr);

                //将取出的LogBuffer写入磁盘文件
                _logFile.append(_writingBuffer->getData(), _writingBuffer->length());

                //归还LogBuffer回内存池
                _objectPool.dealloc(_writingBuffer);

                _writingBuffer= nullptr;
            }
            //刷新磁盘缓存
            _logFile.flush();

        }
    }

    /**
      * 将当前LogBuffer压入队列
      */
    void curBufferFlush(){

        //检测队列容量是否超过限制
        if (_buffQueue.size() > _maxSize)
            return;

        BigBuffer* tmp;
        {
            //更换LogBuffer
            std::unique_lock<std::mutex> locklog(_mutexLog);
            tmp=_curBuffer;
            _curBuffer=_objectPool.alloc();
        }
        {
            //将之前的logBuffer压入队列
            std::unique_lock<std::mutex> lockqueue(_mutexQueue);
            _buffQueue.push(tmp);

        }

    }

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



#endif //BELUGA_ASYNLOGGING_H
