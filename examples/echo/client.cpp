
#include <iostream>
#include <stdio.h>
#include <map>
#include <unistd.h>
#include <poll.h>

#include "beluga/base/Util.h"
#include "beluga/Client.h"

#define OPEN_MAX 200

int main(){
    struct pollfd fds[OPEN_MAX];
    for(int j=0;j<OPEN_MAX;j++){
        fds[j].fd =  -1;
    }

    Client client("10.20.4.5",9000);
    fds[0].fd=client.getFD();
    fds[0].events = POLLIN;

    int timefd=timerfd_init(1000);
    fds[1].fd=timefd;
    fds[1].events = POLLIN;

    fds[2].fd=0;
    fds[2].events = POLLIN;

    for(;;){
        int nready = poll(fds, 3, 10000);

        if(fds[0].revents&(POLLIN)){
            //收到数据
            for(std::string& msg:client.readStream()){
                std::cout<<msg<<std::endl;
            }
        }

        if(fds[1].revents&(POLLIN)){
            // 1ms 定时
            uint64_t exp = 0;
            read(timefd, &exp, sizeof(uint64_t));
            client.write("Hello !!!"); //发送数据
        }
        if(fds[2].revents&(POLLIN)){
            //发送数据
            char sendBuffer[100];
            std::cout<<"send data"<<std::endl;
            int len=read(0,sendBuffer,100);
            client.write(std::string(sendBuffer,sendBuffer+len));
        }

    }

}