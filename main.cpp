
#include <iostream>
#include<cstdio>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<cstring>
#include<arpa/inet.h>
#include "Server.h"
//#include "Alloctor.h"

/*第一次测试*/
using namespace std;
int main() {

    Server server(9000);
    server.startListen();


    return 0;
}