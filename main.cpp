
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
#include "LogStream.h"


std::string onMessage(std::string msg){
    return msg;
}

using namespace std;
int main() {
    LOG.resetPath("/home/zhsy/work/ClionWorkspace/beluga/log/LogTest.log");
    Server server(9000);
    server.setMessageCallBack(onMessage);
    server.startListen();
    return 0;
}