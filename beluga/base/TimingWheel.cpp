//
// Created by zhsy on 19-7-8.
//

#include "TimingWheel.h"
#include <cassert>
TimingWheel::TimingWheel(int size):_curBasket(new Basket()),_size(1){
    _curBasket->_next=_curBasket;
    _curBasket->_pre=_curBasket;
    for(int i=0;i<size-1;i++){
        addBasket();
    }
}
TimingWheel::~TimingWheel(){

}
std::unordered_set<int> TimingWheel::step(){

    std::lock_guard<std::mutex> lock(_mutex);
    //printBaskets();
    _curBasket=_curBasket->_next;
    std::unordered_set<int> removedConnFDs;
    removedConnFDs.swap(_curBasket->_connFdSet);
    for(auto conn:removedConnFDs){
        _connMap.erase(conn);
    }
    return removedConnFDs;
}
void TimingWheel::printBaskets(){
    Basket* basket=_curBasket;
    for(int i=0;i<_size;i++){
        std::cout<<"---"<<basket->_connFdSet.size();
        basket=basket->_next;
    }
    std::cout<<"---"<<std::endl;
}
int TimingWheel::addConn(int connfd){
    std::lock_guard<std::mutex> lock(_mutex);
    assert(_connMap.find(connfd)==_connMap.end());
    _connMap[connfd]=_curBasket;
    _curBasket->_connFdSet.insert(connfd);
    return 0;
}
int TimingWheel::updateConn(int connFd){
    std::lock_guard<std::mutex> lock(_mutex);
    if(_connMap.find(connFd)==_connMap.end()){
        //连接在时间轮里不存在
        std::cout<<"Warning: Add a connection when update timing wheel"<<std::endl;
        _connMap[connFd]=_curBasket;
        _curBasket->_connFdSet.insert(connFd);
        return 0;
    }else if(_connMap[connFd]!=_curBasket){
        //连接在一秒钟之内未更新过
        _connMap[connFd]->_connFdSet.erase(connFd);
        _curBasket->_connFdSet.insert(connFd);
        _connMap[connFd]=_curBasket;
        return 0;
    }
    else{
        //连接在一秒钟之内更新过
        return -1;
    }
}
void TimingWheel::addBasket(){
    Basket *newBasket=new Basket();
    Basket *nextBasket=_curBasket->_next;
    _curBasket->_next=newBasket;
    newBasket->_pre=_curBasket;
    newBasket->_next=nextBasket;
    nextBasket->_pre=newBasket;
    _size++;
}