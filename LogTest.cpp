//
// Created by zhsy on 19-7-10.
//

#include "LogStream.h"
int main(){


    /*AsynLogging asynLogging;

    asynLogging.setLogPath("/home/zhsy/work/ClionWorkspace/beluga/log/LogTest.log");
    asynLogging.startLoop();

    std::string s="jfdksjfkliseurmlkjfdogidsjoig\n";
    int sz=s.length();
    size_t count=0;
    while(1){

        asynLogging.append(s.c_str(),s.length());
        std::cout<<(count+=1)<<std::endl;


    }*/
   // LogStream logStream;
    LOG.resetPath("/home/zhsy/work/ClionWorkspace/beluga/log/LogTest.log");
    size_t count=0;

    while(1){
        LOG<<"Hello MyLog"<<"Test int"<<53<<"Test double="<<47.213<<"\n";
        std::cout<<count++<<std::endl;
    }

}