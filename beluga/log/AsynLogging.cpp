/**
 * @author Zhang Senyan
 * Date: 2019-06-25
 *
 */

#include "AsynLogging.h"

//构造函数
AsynLogging::AsynLogging():
            _objectPool(),
            _curBuffer(nullptr),
            _maxSize(100),
            _runing(false),
            _t(std::mem_fun(&AsynLogging::HandleLoop),this),
            _logFile(){

    _curBuffer=_objectPool.alloc();
}

AsynLogging::~AsynLogging(){

}

/**
 * 设置log输出文件
 * 转调用logFile相关文件
 * @param logPath ： log文件路径
 */
void AsynLogging::setLogPath(std::string logPath){
    _logFile.resetLogPath(logPath);
}

/**
 * 启动日志线程
 */
void AsynLogging::startLoop(){
    _t.detach();
    _runing=true;
}

/**
 * 将日志文件写入buffer
 * @param ch 待写入字符串
 * @param len 字符串长度
 * @return false ： 写入失败
 */
bool AsynLogging::append(const char *ch,size_t len){

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
 * 日志线程主循环
 * 1. 等待日志队列，有日志到来就立刻写入磁盘
 * 2. 定时检测日志缓冲区，如果一直没有写满，超过时间限制，就自动写入磁盘
 */
void AsynLogging::HandleLoop(){
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
void AsynLogging::curBufferFlush(){

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