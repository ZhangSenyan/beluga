/**
 * @author Zhang Senyan
 * Date: 2019-04-20
 *
 */

#include <cassert>

#include "AcceptThread.h"

/**
 * 构造函数
 * @param size ： 允许挂载的连接数上限
 * @param timer_ms ： 定时时间
 */
AcceptThread::AcceptThread(int size,int timer_ms):
                EventLoop(size,timer_ms),
                _timingWheel(10),
                _timeCount(0){

    std::cout<<"AcceptThread::AcceptThread"<<std::endl;
}
AcceptThread::~AcceptThread(){

}

/**
 * 添加一个新客户端连接
 * @param conn
 */
void AcceptThread::addConnction(std::shared_ptr<Connection> conn){

    //设置Channel
    conn->getChannel()->setHolder(conn);
    conn->getChannel()->setEvents(EPOLLIN | EPOLLET);

    assert(connMap.find(conn->getFd())==connMap.end());

    //将连接加入时间轮
    _timingWheel.addConn(conn->getFd());

    //注册到connMap
    connMap[conn->getFd()]=conn;

    //添加到epoll
    addChannel(conn->getChannel());

}

/**
 * 移除客户端连接
 * @param conn
 */
void AcceptThread::removeConnction(std::shared_ptr<Connection> conn){

    //Drop 用户回调
    _onDropConntion(conn);

    //从epoll中移除
    removeChannel(conn->getChannel());

    //注销fd
    connMap.erase(conn->getFd());
}
void AcceptThread::removeConnction(int connfd){
    /**
     *仅仅是关闭对应的消息接收，具体文件描述符关闭要等到全部消息队列中该连接消息清空
     *时间轮中无需清理，心跳自检程序会自动清除
     *
     * removeChannel应该在erase前
     */
    removeChannel(connMap[connfd]->getChannel());
    connMap.erase(connfd);

}

/**
 * 获取连接注册信息
 * @return connMap
 */
std::map<int,std::shared_ptr<Connection>>& AcceptThread::getConnMap(){
    return connMap;
}

/**
 * 定时处理函数
 * 定时时间： constructor -> timer_ms
 *          默认 50ms
 */
void AcceptThread::timerHandle() {

    _timeCount+=50;

    // 50ms定时任务
    timerHandle50();

    if(_timeCount>=1000){
        _timeCount=0;
        // 1s 定时任务
        timerHandle1000();
    }
}

/**
 * 1s 定时任务
 * 用于心跳检测
 */
void AcceptThread::timerHandle1000() {

    //时间轮向后移动一个节点
    std::unordered_set<int> dlconns=_timingWheel.step();

    //移除连接 放在所有任务执行完再执行
    addPendingFunctor([dlconns,this](){
        for(auto &dlconnfd:dlconns){
            removeConnction(dlconnfd);
        }
    });

}

/**
 * 50ms 计时任务
 * 用于定时发送数据
 * 如果50ms内缓冲区都未被写满，那么需要冲刷缓冲区
 */
void AcceptThread::timerHandle50() {

    //对于所有连接，冲刷缓冲区
    for(auto &connPair:connMap){
        connPair.second->flushBuffer();
    }
}

/**
 * 设置消息队列
 * @param taskQueue
 */
void AcceptThread::setTaskQueue(const std::shared_ptr<TaskQueue>& taskQueue){
    _taskQueue=taskQueue;
}
std::shared_ptr<TaskQueue> AcceptThread::getTaskQueue(){
    return _taskQueue;
}

/**
 * 获取本线程当前连接数
 * @return 连接对应的fd
 */
int AcceptThread::getConnSize() const{
    return connMap.size();
}

/**
 * 心跳检测
 * ------------------------------
 * 当该连接活动时，通过调用该函数，
 * 将connetion移动到当前时间轮节点中
 * ------------------------------
 * @param conn : 要更新的连接，智能指针形式传入
 * @return -1 更新失败
 */
int AcceptThread::updateConn(Connection::ConnPtr conn){
    return _timingWheel.updateConn(conn->getFd());
}

/**
 * 设置掉线回调
 * @param dropFunctor
 */
void AcceptThread::setDropConnectionCallBack(beluga::ConnFunctor dropFunctor){
    _onDropConntion=dropFunctor;
}