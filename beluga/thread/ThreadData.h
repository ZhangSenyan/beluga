/**
 * @author Zhang Senyan
 * Date: 2019-04-07
 *
 * ThreadData
 */

#ifndef BELUGA_THREADDATA_H
#define BELUGA_THREADDATA_H


#include <functional>

class ThreadData {
public:

    typedef std::function<void()> ThreadFunc;
    ThreadData(ThreadFunc tf):_theadFunc(tf){}

    ThreadFunc _theadFunc;
};


#endif //BELUGA_THREADDATA_H
