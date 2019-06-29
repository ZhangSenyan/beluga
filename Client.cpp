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

    char sendbuff[100]={0};
    char recvbuff[100]={0};
    ssize_t recvlen=0;
    while(1){
        cin>>sendbuff;
        cout<<strlen(sendbuff)<<endl;
        write(fd,sendbuff,strlen(sendbuff));
        //recvlen=read(fd,recvbuff,sizeof(recvbuff));
        //cout<<recvlen<<endl;
        //cout<<recvbuff<<endl;
    }
}