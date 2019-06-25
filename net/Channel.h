//
// Created by zhsy on 19-6-16.
//

#ifndef HCCSERVER_CHANNEL_H
#define HCCSERVER_CHANNEL_H


#include <functional>

class Channel {

public:
    typedef std::function<void(void)> CallFunc;
    Channel(int fd):_fd(fd){

    }
    ~Channel(){

    }
private:
    CallFunc _readhandler;
    CallFunc _writehandler;
    CallFunc _errorhandler;
    int _fd;
    __uint32_t _events;
    __uint32_t _revents;

public:
    void setReadHandler(CallFunc readhandler){
        _readhandler=readhandler;
    }
    void setWriteHandler(CallFunc writehandler){
        _writehandler=writehandler;
    }
    void setErrorHandler(CallFunc errorhandler){
        _errorhandler=errorhandler;
    }
    void handleEvents(){

    }
    int getFd(){
        return _fd;
    }
    __uint32_t getEvents(){
        return  _events;
    }
    void setEvents(__uint32_t events){
        _events=events;
    }
    __uint32_t getRevents(){
        return  _revents;
    }
    void setRevents(__uint32_t revents){
        _revents=revents;
    }


};


#endif //HCCSERVER_CHANNEL_H




