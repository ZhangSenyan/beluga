/**
 * @author Zhang Senyan
 * Date: 2019-04-15
 *
 */


#include <functional>
#include <memory>
#include <cassert>

#include "Connection.h"
#include "beluga/thread/AcceptThread.h"
#include "beluga/task/CellTask.h"

/**
 * 构造函数
 * @param fd  ：  accept返回的文件描述符
 * @param clientAddr  ： 客户端地址
 */
Connection::Connection(int fd,struct sockaddr_in clientAddr):
                        _fd(fd),
                        _clientAddr(clientAddr),
                        _acceptThread(),
                        _channel(new Channel(fd)),
                        _buffer(fd,this){

    //设置事件处理函数
    _channel->setReadHandler(std::bind(&Connection::handleRead,this));
    _channel->setWriteHandler(std::bind(&Connection::handleWrite,this));
    _channel->setErrorHandler(std::bind(&Connection::handleError,this));
    //_channel->setHolder(shared_from_this());

}


Connection::~Connection() {
    close(_fd);
}


int Connection::getFd() const{
    return _fd;
}


std::string Connection::getIP() const{
    return inet_ntoa(_clientAddr.sin_addr);
}

int Connection::getPort() const{
    return ntohs(_clientAddr.sin_port);
}

/**
 * 读事件处理函数
 */
void Connection::handleRead(){

    assert(!_acceptThread.expired());

    //获取任务队列
    std::shared_ptr<AcceptThread> acceptThread=_acceptThread.lock();
    std::shared_ptr<TaskQueue> taskQueen=acceptThread->getTaskQueue();

    //更新时间轮-心跳检测
    acceptThread->updateConn(shared_from_this());

    //读取消息
    std::vector<std::string> msgs=_buffer.readStream();
    
    for(auto msg:msgs){

        //过滤心跳信息
        
        if(msg!="^^^") {

            //创建任务并压入队列
            std::shared_ptr<CellTask> cellTaskptr(new CellTask(msg, shared_from_this()));
            taskQueen->push(cellTaskptr);
        }

    }
    //std::string msg=_buffer.readSimple();
    //std::cout<<msg<<std::endl;
}

/**
 * 写事件处理函数
 */
void Connection::handleWrite(){
    //std::cout<<"handle write ... "<<std::endl;
    _buffer.flushSend();
    if(_buffer.empty()){
        closeListenEvent();
    }
}

void Connection::handleError() {
    std::cout<<"handle error ... "<<std::endl;
}

/**
 * @return Connection 对应的 Channel
 */
std::shared_ptr<Channel> Connection::getChannel(){
    return _channel;
}

/**
 * 将缓冲区数据发送给客户端
 * @return 发送的字节数
 */
int Connection::flushBuffer(){
    return _buffer.flushSend();
}

std::shared_ptr<AcceptThread> Connection::getAcceptThread(){
    return _acceptThread.lock();
}

/**
 * 监听该 Connection 是否可写
 */
void Connection::openListenEvent(){
    _channel->addEvents(EPOLLOUT);
    getAcceptThread()->getEpoll()->updateChannel(_channel);
}

void Connection::closeListenEvent(){
    _channel->removeEvents(EPOLLOUT);
    getAcceptThread()->getEpoll()->updateChannel(_channel);
}

/**
 * 将响应消息写入发送缓冲区
 * @param result
 * @return
 */
void Connection::writeBuffer(std::string result){

    _buffer.write(result);
}

/**
 * 设定所属线程
 * @param acceptThread
 */
void Connection::setAcceptThread(std::shared_ptr<AcceptThread> acceptThread){
    _acceptThread=acceptThread;
}