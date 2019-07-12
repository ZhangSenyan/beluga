//
// Created by zhsy on 19-7-8.
//

#ifndef HCCSERVER_TIMINGWHEEL_H
#define HCCSERVER_TIMINGWHEEL_H

#include <unordered_map>
#include <mutex>
#include "beluga/base/Basket.h"


class TimingWheel {
public:
    explicit TimingWheel(int size);
    ~TimingWheel();
    std::unordered_set<int> step();
    void printBaskets();
    int addConn(int connfd);
    int updateConn(int connFd);

private:
    Basket* _curBasket;
    int _size;
    std::unordered_map<int,Basket*> _connMap;
    std::mutex  _mutex;
private:
    void addBasket();
};


#endif //HCCSERVER_TIMINGWHEEL_H
