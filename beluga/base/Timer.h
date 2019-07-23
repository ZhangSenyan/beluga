/**
 * @author Zhang Senyan
 * Date: 2019-06-12
 *
 * @details 定时器
 *
 */

#ifndef BELUGA_TIMER_H
#define BELUGA_TIMER_H


#include <memory>
#include <functional>

class EventLoop;
class Channel;

class Timer : std::enable_shared_from_this<Timer>{
public:
    //构造函数
    explicit Timer(int ms);

    ~Timer();

    //获取 timefd
    int getFD();

    //设定所属线程
    void setHolder(EventLoop* acceptThread);

    //时间处理函数
    void handleRead();
    void handleWrite();
    void handleError();

    //获取定时器所属channle
    std::shared_ptr<Channel> getChannel();

    //设置定时处理函数
    void setTimeHandler(std::function<void(void)> timeHandler);

    //获取当前时间
    static std::string getCurrentTime();

private:
    int _timefd;

    //间隔定时时间
    int _interTime;

    //定时器对应的channel
    std::shared_ptr<Channel> _channel;

    EventLoop* _eventLoop;

    //定时处理函数
    std::function<void(void)> _timeHandler;

};


#endif //BELUGA_TIMER_H
