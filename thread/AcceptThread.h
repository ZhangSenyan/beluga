//
// Created by zhsy on 19-6-8.
//

#ifndef HCCSERVER_ACCEPTTHREAD_H
#define HCCSERVER_ACCEPTTHREAD_H

#include "../net/Epoll.h"
#include "ThreadData.h"
#include <iostream>
#include <pthread.h>

#include <unistd.h>
#include <thread>


class AcceptThread {
public:
    AcceptThread():_epoll(10000),_runing(false),_t(std::mem_fun(&AcceptThread::HandleLoop),this){

    }
    ~AcceptThread(){

    }
    void startLoop(){
        _runing=true;
        _t.detach();


    }
    void HandleLoop(){
        while(_runing){

            std::cout<<"Looping ..."<<std::endl;
            Epoll::VectorCh channels=_epoll.poll();
            for(auto channel:channels){
                channel->handleEvents();
            }
            sleep(1);
        }
    }
    void addChannel(Epoll::ptrChannel channel){
        _epoll.addChannel(channel);
    }
private:
    Epoll _epoll;
    bool _runing;
    std::thread _t;

};


#endif //HCCSERVER_ACCEPTTHREAD_H
