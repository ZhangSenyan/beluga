
#include <iostream>
#include <stdio.h>
#include <map>
#include <unistd.h>
#include <poll.h>

#include "beluga/base/Util.h"
#include "beluga/Client.h"

using namespace std;
#define OPEN_MAX 2000

int main(){
    struct pollfd fds[OPEN_MAX];
    for(int j=0;j<OPEN_MAX;j++){
        fds[j].fd =  -1;
    }

    std::shared_ptr<Client> pClient(new Client());
    fds[0].fd=pClient->getFD();
    fds[0].events = POLLIN;

    int timefd=timerfd_init(1000);
    fds[1].fd=timefd;
    fds[1].events = POLLIN;

    fds[2].fd=0;
    fds[2].events = POLLIN;

    for(;;){
        int nready = poll(fds, 3, 10000);

        /*if(fds[0].revents&(POLLIN)){
            //收到数据
            std::cout<<"recv data"<<std::endl;
            pClient->readStream();
            //for(std::string& msg:pClient->readStream()){
            //    std::cout<<msg<<std::endl;
            //}
        }*/

        if(fds[1].revents&(POLLIN)){
            // 1ms 定时
            uint64_t exp = 0;
            read(timefd, &exp, sizeof(uint64_t));
            std::cout<<"timer task"<<std::endl;
            pClient->write("Hello haha~");
        }
        if(fds[2].revents&(POLLIN)){
            //发送数据
            std::cout<<"send data"<<std::endl;
            std::string str;
            cin>>str;
            pClient->write(str);
        }

    }
}