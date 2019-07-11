//
// Created by zhsy on 19-7-9.
//
#include <iostream>
#include <memory>
#include <vector>
#include "Alloctor.h"
//#include "MemoryBlock.h"
int main(){
    std::shared_ptr<int> a;
    std::cout<<"a"<<std::endl;
    a=std::make_shared<int>(new int(8));
    std::cout<<"b"<<std::endl;
    std::weak_ptr<int> p;
    std::cout<<"c"<<std::endl;
    p=a;
    std::cout<<"d"<<std::endl;
    std::shared_ptr<int> b=p.lock();
    std::cout<<"e"<<std::endl;
    std::string s="ajklsdfj";
    s+="jsldkfjowaiewrj";
    std::cout<<"f"<<std::endl;
    std::vector<int> v;
    for(int i=0;i<10;i++)
        v.push_back(i);
    std::cout<<"end()"<<std::endl;
}