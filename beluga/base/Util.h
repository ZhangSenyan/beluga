/**
 * @author Zhang Senyan
 * Date: 2019-04-03
 *
 * 常用工具
 */

#ifndef BELUGA_UTIL_H
#define BELUGA_UTIL_H


#include <cstdio>

extern int socketCreate(int port);

extern int setnonblocking(int sockfd);

extern ssize_t readn(int fd,char *ptr,size_t n);

extern ssize_t writen(int fd,char *ptr,size_t n);

extern int timerfd_init(unsigned long ms);

extern std::string getCWD();

extern int setblocking(int sockfd);

#endif //BELUGA_UTIL_H
