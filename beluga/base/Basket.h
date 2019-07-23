/**
 * @author Zhang Senyan
 * Date: 2019-06-10
 */

#ifndef HCCSERVER_BASKET_H
#define HCCSERVER_BASKET_H

#include <unordered_set>
#include <memory>
#include "beluga/net/Connection.h"


/**
 * TimingWheel 中的 Node
 * 由 一个ListNode 和 一个hashmap 组成
 */

struct Basket {

    //_next 指向下一个篮子
    Basket *_next;

    //_pre 指向下一个篮子
    Basket *_pre;

    /**
     * _connFdSet： 存在当前节点里所有 Connection 的集合
     * 集合中存储的是 Connection 的 fd
     *
     * 当收到 Connection 的心跳时，将Connection从上一个Node中取出
     * 存入当前的时间戳对应的 Node 里
     */
    std::unordered_set<int> _connFdSet;

};


#endif //HCCSERVER_BASKET_H
