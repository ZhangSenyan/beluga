
#include <iostream>
#include<cstdio>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<cstring>
#include<arpa/inet.h>

/*第一次测试*/
using namespace std;
int main() {

    int fd=socket(AF_INET,SOCK_STREAM,0);
    if(fd==-1){
        perror("SOCKET Create");
    }
    printf("1\n");
    struct sockaddr_in server_addr, client_addr;
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(10000);
    server_addr.sin_addr.s_addr=inet_addr("10.20.5.3");
    if(-1==bind(fd,(struct sockaddr*)&server_addr,sizeof(server_addr)))
        perror("SOCKET BIND");
    printf("2\n");
    if(-1==listen(fd,SOMAXCONN))
        perror("SOCKET LISTEN");
    socklen_t socklen=sizeof(struct sockaddr_in);
    int conn;
    memset(&client_addr,0,sizeof(struct sockaddr_in));
    int endflag=0;
    pid_t accept_thread;
    printf("3\n");
    conn=accept(fd,(struct sockaddr*)&client_addr,&socklen);
    char recvbuff[100]={0};
    char sendbuff[100]={0};
    ssize_t recvlen=0;
    printf("4\n");
    while(1){
        recvlen=read(conn,recvbuff,sizeof(recvbuff));
        write(conn,recvbuff,recvlen);
        printf("%s\n",recvbuff);
        printf("5\n");
    }



    return 0;
}