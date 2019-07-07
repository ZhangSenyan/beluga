//
// Created by zhsy on 19-7-3.
//

#ifndef HCCSERVER_TIMER_H
#define HCCSERVER_TIMER_H


#include <memory>
#include <functional>


class EventLoop;
class Channel;
class Timer :std::enable_shared_from_this<Timer>{
public:
    explicit Timer(int ms);
    ~Timer();
    int getFD();
    void setHolder(EventLoop* acceptThread);
    void handleRead();
    void handleWrite();
    void handleError();
    std::shared_ptr<Channel> getChannel();
    void setTimeHandler(std::function<void(void)> timeHandler);
private:
    int timefd;
    int interTime;
    std::shared_ptr<Channel> _channel;
    EventLoop* _eventLoop;
    std::function<void(void)> _timeHandler;
};


#endif //HCCSERVER_TIMER_H
