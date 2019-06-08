//
// Created by zhsy on 19-6-8.
//

#include "Connection.h"


Connection::Connection(){

}

int Connection::getFd(){
    return _fd;
}

void Connection::setFd(int fd){
    _fd=fd;
}
void Connection::handleRead(){

}
void Connection::handleWrite(){

}