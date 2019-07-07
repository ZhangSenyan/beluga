
#include <iostream>
#include <stdio.h>
#include <map>
#include <unistd.h>
#include <poll.h>

#include "Util.h"
#include "Client.h"

using namespace std;
#define OPEN_MAX 50000

int main(){
    struct pollfd fds[OPEN_MAX];
    for(int j=0;j<OPEN_MAX;j++){
        fds[j].fd = -1;
    }

    std::map<int,std::shared_ptr<Client>> clientSet;
    int i=0;
    for(;i<OPEN_MAX-10;i++){
        std::shared_ptr<Client> pClient(new Client());
        clientSet[pClient->getFD()]=pClient;
        fds[i].fd=pClient->getFD();
        fds[i].events = POLLIN;
    }
    int timefdIndex=i++;
    int timefd=timerfd_init(1000);
    fds[timefdIndex].fd=timefd;
    fds[timefdIndex].events = POLLIN;
    int count=0;
    for(;;){
        int nready = poll(fds, i+1, 10000);

        if(fds[timefdIndex].revents&(POLLIN)){
            uint64_t exp = 0;
            read(timefd, &exp, sizeof(uint64_t));

            cout<<"Recv Msg mum="<<count<<endl;
            count=0;
            for(auto pair:clientSet){
                for(int c=0;c<10;c++)
                    pair.second->write("abcdefghigh");
            }
        }
        for(int i=0;i<OPEN_MAX;i++){

            if(fds[i].fd<=0)
                continue;
            if((fds[i].revents & (POLLIN)) && (clientSet.find(fds[i].fd)!=clientSet.end())){
                count+=clientSet[fds[i].fd]->readAndDeal();
            }
        }
    }
}