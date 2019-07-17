# beluga (a simple C++ network library) 
[![build status](https://travis-ci.org/ZhangSenyan/beluga.svg?branch=master)](https://travis-ci.org/ZhangSenyan/beluga)
[![license](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-Linux-green.svg?style=flat)](https://github.com/SenyanZhang/beluga)

# Introduction
beluga是一款小型的C++网络库，支持高并发，多线程，用于实现高性能网络服务。
beluga 基于事件驱动，采用非阻塞IO，整体框架由mainReactor,SubReactor,Worker Threads Pool三部分组成。
# Technical points

* 基于Reactor模式，包含监听线程，IO线程和计算线程，监听线程负责监听和创建连接，IO线程负责读取写入网络数据，
计算线程负责具体业务逻辑
* IO线程采用EPOLL边沿触发多路复用技术，非阻塞IO
* 主线程接收创建连接后以Least Connections方式，分发给IO线程
* 采用双任务队列机制，实现紧急任务优先处理
* 基于时间轮机制和哈希表实现心跳检测，剔除超时连接
* 为了避免内存泄漏和野指针，重复delete等问题，对于线程间对象使用智能指针等RALL机制
* 支持定时定量发送数据
* 采用异步日志系统，基于buffer对象池实现日志快速写入
* 内存池采用动态倍增扩容机制以及lazy加载模式，最大限度节省内存开销，提高内存申请速率
* 采用多线程和线程池，充分利用多核CPU，减少线程频繁创建带来的开销


# Model
## Multiple Reactors
使用multiple Reactors (MainReactor+SubReactor+Worker Threads Pool)
![Multiple Reactors](https://github.com/ZhangSenyan/beluga/raw/master/docs/MultipleReactors.png)

# Build

```cpp
git clone --recursive https://github.com/SenyanZhang/beluga.git
cd beluga
mkdir build
cd build
cmake ..
make
```



# Log

# Refrence

主要参考[muduo],并添加了一些自己的设计，底层代码大部分是自己实现的。

另外也参考了[linyacool-WebServer],[evpp],[libevent]等。

[linyacool-WebServer]:https://github.com/linyacool/WebServer

[muduo]:https://github.com/chenshuo/muduo
[libevent]:https://github.com/libevent/libevent
[asio]:http://www.boost.org/
[evpp]:https://github.com/Qihoo360/evpp
