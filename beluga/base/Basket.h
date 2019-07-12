//
// Created by zhsy on 19-5-8.
//

#ifndef HCCSERVER_BASKET_H
#define HCCSERVER_BASKET_H

#include <unordered_set>
#include <memory>
#include "beluga/net/Connection.h"

struct Basket {
    //typedef std::shared_ptr<Basket> BasketPtr;
    Basket *_next;
    Basket *_pre;
    std::unordered_set<int> _connFdSet;
};


#endif //HCCSERVER_BASKET_H
