/**
 * @author Zhang Senyan
 * Date: 2019-06-25
 *
 * 常用工具
 */
#ifndef HCCSERVER_ASYNLOGGING_H
#define HCCSERVER_ASYNLOGGING_H

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
    typedef LogBuffer<4096> BigBuffer;
    AsynLogging();
    ~AsynLogging();
    void setLogPath(std::string logPath);
    void startLoop();
    bool append(const char *ch,size_t len);

    template <typename T>
    bool append(T v);

    void HandleLoop();
    void curBufferFlush();

private:
    std::queue<BigBuffer*> _buffQueue;
    BigBuffer* _curBuffer;
    BigBuffer* _writingBuffer;
    ObjectPool<BigBuffer> _objectPool;
    std::mutex _mutexQueue;
    std::mutex _mutexLog;
    std::condition_variable _cv;
    int _maxSize;
    bool _runing;
    std::thread _t;
    LogFile _logFile;
    bool update;
};

template <typename T>
bool AsynLogging::append(T v){
    std::unique_lock<std::mutex> locklog(_mutexLog);

    if(-1==_curBuffer->append(v)){
        //写入失败
        {

            std::unique_lock<std::mutex> lockqueue(_mutexQueue);

            if (_buffQueue.size() > _maxSize)
                return false;
            _buffQueue.push(_curBuffer);
            _curBuffer=_objectPool.alloc();

        }
        _cv.notify_all();
        _curBuffer->append(v);
        //assert(-1!=_curBuffer->append(v));
    }

}

#endif //HCCSERVER_ASYNLOGGING_H
