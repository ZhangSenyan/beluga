#include <iostream>
#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<string.h>
#include<arpa/inet.h>

using namespace std;
int main() {

    int fd=socket(AF_INET,SOCK_STREAM,0);
    if(fd==-1){
        perror("SOCKET Create");
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(10002);
    server_addr.sin_addr.s_addr=inet_addr("10.20.4.5");
    if(-1==bind(fd,(struct sockaddr*)&server_addr,sizeof(server_addr)))
        perror("SOCKET BIND");

    if(-1==listen(fd,SOMAXCONN))
        perror("SOCKET LISTEN");
    
    struct sockaddr_in client_addr;
    memset(&client_addr,0,sizeof(struct sockaddr_in));
    socklen_t socklen=sizeof(struct sockaddr_in);
    int conn;

    conn=accept(fd,(struct sockaddr*)&client_addr,&socklen);
    cout<<"Get a connect:"<<endl;

    char recvbuff[100]={0};
    ssize_t recvlen=0;

    while(1){
        recvlen=read(conn,recvbuff,sizeof(recvbuff));
        write(conn,recvbuff,recvlen);
        printf("%s\n",recvbuff);
    }




    return 0;
}