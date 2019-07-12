//
// Created by zhsy on 19-7-10.
//

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
    AsynLogging():_objectPool(),_curBuffer(nullptr),_maxSize(100),_runing(false),
    _t(std::mem_fun(&AsynLogging::HandleLoop),this),_logFile(){

        _curBuffer=_objectPool.alloc();
    }
    ~AsynLogging(){

    }
    void setLogPath(std::string logPath){
        _logFile.resetLogPath(logPath);
    }
    void startLoop(){
        _t.detach();
        _runing=true;
    }
    bool append(const char *ch,size_t len){

        std::unique_lock<std::mutex> locklog(_mutexLog);

        if(-1==_curBuffer->append(ch,len)){
            //写入失败
            {

                std::unique_lock<std::mutex> lockqueue(_mutexQueue);

                if (_buffQueue.size() > _maxSize)
                    return false;
                _buffQueue.push(_curBuffer);
                _curBuffer=_objectPool.alloc();
            }
            _cv.notify_all();
            _curBuffer->append(ch,len);
            //assert(-1!=_curBuffer->append(ch,len));
        }

    }
    template <typename T>
    bool append(T v){
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

    void HandleLoop(){
        while (!_runing)
            usleep(1000);
        while (_runing){
            {
                std::unique_lock<std::mutex> lckQ(_mutexQueue);
                if (_buffQueue.empty()) {
                    _cv.wait_for(lckQ, std::chrono::seconds(3));
                }
            }
            if (_buffQueue.empty()) {
                curBufferFlush();
            }
            while(!_buffQueue.empty()){
                {
                    std::unique_lock<std::mutex> lckQ(_mutexQueue);
                    _writingBuffer = _buffQueue.front();
                    _buffQueue.pop();
                }
                assert(_writingBuffer!= nullptr);

                _logFile.append(_writingBuffer->getData(), _writingBuffer->length());
                _objectPool.dealloc(_writingBuffer);
                _writingBuffer= nullptr;
            }
            _logFile.flush();
        }
    }
    void curBufferFlush(){
        if (_buffQueue.size() > _maxSize)
            return;

        BigBuffer* tmp;
        {
            std::unique_lock<std::mutex> locklog(_mutexLog);
            tmp=_curBuffer;
            _curBuffer=_objectPool.alloc();
        }
        {
            std::unique_lock<std::mutex> lockqueue(_mutexQueue);


            _buffQueue.push(tmp);


        }

    }
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


#endif //HCCSERVER_ASYNLOGGING_H
