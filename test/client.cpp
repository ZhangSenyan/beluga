#include <iostream>
#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<string.h>
#include<stdio.h>
#include<arpa/inet.h>
#include <poll.h>

#include "base/Buffer.h"
#include "base/Util.h"

#define OPEN_MAX 4096
using namespace std;
int main(){
    int fd=socket(AF_INET,SOCK_STREAM,0);
    if(fd==-1){
        perror("SOCKET Create");
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(10000);
    server_addr.sin_addr.s_addr=inet_addr("10.20.4.5");

    int ret=connect(fd,(const struct sockaddr*) &server_addr,sizeof(struct sockaddr));
    cout<<ret<<endl;
    setnonblocking(fd);


    Buffer buffer(fd);

    struct pollfd fds[OPEN_MAX];

    for (int i = 0; i < OPEN_MAX; i++)
        fds[i].fd = -1;      /* -1 indicates available entry */
    fds[0].fd = 0;
    fds[0].events = POLLIN;
    fds[1].fd=fd;
    fds[1].events = POLLIN;
    cout<<fd<<endl;
    for(;;){
        int nready = poll(fds, 3, 10000);
        //cout<<"---------...........-----------......."<<endl;
        if(fds[0].revents & (POLLIN |POLLERR)){
            std::string content;
            cin>>content;
            buffer.write(content);
            buffer.flushSend();
            cout<<"send buffer"<<content<<endl;
        }
        if(fds[1].revents & POLLIN ){
            cout<<"recv Msg"<<endl;
            auto recvV=buffer.readStream();
            for(auto msg:recvV){
                cout<<msg<<endl;
            }
        }
        if(fds[1].revents & POLLERR ){
            cout<<"POLL ERR ..."<<endl;
            //auto recvV=buffer.readStream();
            //for(auto msg:recvV){
            //cout<<msg<<endl;
            //}
        }
        cout<<"end"<<endl;

    }
}