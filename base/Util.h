//
// Created by zhsy on 19-6-8.
//

#ifndef HCCSERVER_UTIL_H
#define HCCSERVER_UTIL_H


#include <stdio.h>

extern int socketCreate(int port);
extern int setnonblocking(int sockfd);
extern ssize_t readn(int fd,char *ptr,size_t n);
extern ssize_t writen(int fd,char *ptr,size_t n);
extern int timerfd_init(unsigned long ms);

#endif //HCCSERVER_UTIL_H
