/**
 * @author Zhang Senyan
 * Date: 2019-06-10
 *
 * 时间轮：用于心跳检测
 */


#include <cassert>

#include "beluga/base/TimingWheel.h"
#include "beluga/log/LogStream.h"

/**
 * 构造函数
 * @param size 时间轮中节点数量
 */
TimingWheel::TimingWheel(int size):
    _curBasket(new Basket()),
    _size(1){

    //当只有一个篮子时，将 _next, _pre 都指向自身
    //构造环形结构（时间环）
    _curBasket->_next=_curBasket;
    _curBasket->_pre=_curBasket;

    //往时间环中再添加size-1个节点
    for(int i=0;i<size-1;i++){
        addBasket();
    }
}

TimingWheel::~TimingWheel(){

}

/**
 * 时间轮定时任务：每秒向后移动一个节点
 * @return 超时Connection集合
 */
std::unordered_set<int> TimingWheel::step(){

    std::lock_guard<std::mutex> lock(_mutex);

    //格式化打印时间轮
    //printBaskets();

    _curBasket=_curBasket->_next;

    //每个接收线程都有一个单独的时间轮，所以这里不需要加锁
    std::unordered_set<int> removedConnFDs;
    removedConnFDs.swap(_curBasket->_connFdSet);

    //清除超时连接
    for(auto conn:removedConnFDs){
        _connMap.erase(conn);
    }
    return removedConnFDs;
}

/**
 * 格式化打印时间轮状态
 * for debugging
 */
void TimingWheel::printBaskets(){
    Basket* basket=_curBasket;

    //将时间轮中节点从头至尾遍历一遍
    for(int i=0;i<_size;i++){
        std::cout<<"---"<<basket->_connFdSet.size();
        basket=basket->_next;
    }
    std::cout<<"---"<<std::endl;
}

/**
 * 向时间轮中添加一个新的Connection
 * @param connfd： Connection对应的fd
 * @return 添加成功返回 0
 */
int TimingWheel::addConn(int connfd){

    std::lock_guard<std::mutex> lock(_mutex);
    assert(_connMap.find(connfd)==_connMap.end());

    //注册到hashMap中
    _connMap[connfd]=_curBasket;

    //添加到当前节点对应的 hashset 中
    _curBasket->_connFdSet.insert(connfd);
    return 0;
}

/**
 * 更新时间轮中节点的心跳信息
 * @param connfd ： Connection对应的fd
 * @return  0 ：已更新
 *         -1 ：未更新
 */
int TimingWheel::updateConn(int connfd){

    std::lock_guard<std::mutex> lock(_mutex);

    if(_connMap.find(connfd)==_connMap.end()){

        //连接在时间轮里不存在
        //LOG_WARN<<"Add a connection when update timing wheel"<<LOG_ENDL;
        std::cout<<"Warning: Add a connection when update timing wheel"<<std::endl;

        //添加连接
        _connMap[connfd]=_curBasket;
        _curBasket->_connFdSet.insert(connfd);

        return 0;

    }else if(_connMap[connfd]!=_curBasket){

        //连接在一秒钟之内未更新过

        //从上一个篮子中移除
        _connMap[connfd]->_connFdSet.erase(connfd);

        //添加到当前篮子中去
        _curBasket->_connFdSet.insert(connfd);

        //更新hashMap
        _connMap[connfd]=_curBasket;
        return 0;
    }
    else{
        //连接在一秒钟之内更新过
        //不需要处理
        return -1;
    }
}

/**
 * 向时间轮中再增添一个轮子
 * 只用于初始化
 */
void TimingWheel::addBasket(){

    //创建新节点
    Basket *newBasket=new Basket();

    //插入节点
    Basket *nextBasket=_curBasket->_next;
    _curBasket->_next=newBasket;
    newBasket->_pre=_curBasket;
    newBasket->_next=nextBasket;
    nextBasket->_pre=newBasket;

    //更新时间轮大小
    _size++;
}