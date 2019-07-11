//
// Created by zhsy on 19-6-27.
//

#ifndef HCCSERVER_THREADDATA_H
#define HCCSERVER_THREADDATA_H


#include <functional>

class ThreadData {
public:

    typedef std::function<void()> ThreadFunc;
    ThreadData(ThreadFunc tf):_theadFunc(tf){}

    ThreadFunc _theadFunc;
};


#endif //HCCSERVER_THREADDATA_H
