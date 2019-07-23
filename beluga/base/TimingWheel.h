
/**
 * @author Zhang Senyan
 * Date: 2019-06-10
 *
 * 时间轮：用于心跳检测
 *
 * Struct：
 * =========================================================
 * hashmap<fd, Basket*>
 *
 * |basket1|->|basket2|->|basket3|->...->|basketn|->|basket1|
 *     |          |          |               |          |
 *   <hash>     <hash>     <hash>          <hash>     <hash>
 * ----------------------------------------------------------
 *
 * 时间轮包含一个 hashmap 和 n 个篮子，每个篮子里包含一个hashset
 * hashmap中存放每个fd对应的篮子
 * hashset 中存放当前篮子中所有连接的fd
 *
 * 当接收心跳信息后，先去hashmap中找到该连接之前所在的篮子，
 * 然后将该fd添加到当前篮子中，并从之前篮子里移除
 *
 */


#ifndef BELUGA_TIMINGWHEEL_H
#define BELUGA_TIMINGWHEEL_H

#include <unordered_map>
#include <mutex>

#include "beluga/base/Basket.h"


class TimingWheel {
public:
    //构造函数
    explicit TimingWheel(int size);
    ~TimingWheel();

    /**
     * 定时任务跳动一次，向后移动一个篮子
     *
     * @return std::unordered_set<int>
     *         连接超时的文件描述符
     */
    std::unordered_set<int> step();

    //格式化打印出当期篮子变化情况
    void printBaskets();

    //当新的连接来时，加入篮子中
    int addConn(int connfd);

    //接收到心跳后，将当前连接移动到最新的篮子里
    int updateConn(int connFd);

private:

    //当前时间戳下对应的篮子
    Basket* _curBasket;

    /**
     * 时间轮中篮子的数量
     * 一个有效连接的时间= Timer::_interTime × TimingWheel::_size
     */
    int _size;

    //记录每个连接所处的篮子，用于更新连接状态
    std::unordered_map<int,Basket*> _connMap;
    std::mutex  _mutex;

private:
    //增加时间轮中添加一个新的篮子
    void addBasket();
};


#endif //BELUGA_TIMINGWHEEL_H
