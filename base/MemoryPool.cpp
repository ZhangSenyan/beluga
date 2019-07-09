//
// Created by zhsy on 19-6-8.
//
#include <iostream>
#include "MemoryPool.h"
MemoryPool::MemoryPool(){
    std::cout<<"MemoryPool::MemoryPool()"<<std::endl;
    for(int i=0;i<16;i++){
        _blockArray[i]=new MemoryBlock((i+1)*8);
    }
}
MemoryPool::~MemoryPool(){

}


MemoryPool& MemoryPool::instance(){
    static MemoryPool memoryPool;
    return memoryPool;
}

void* MemoryPool::alloc(size_t size){

    if(size>=128){
        MemoryUnit* p= (MemoryUnit*)::malloc(size+sizeof(MemoryUnit));
        p->holder= nullptr;
        p->next= nullptr;
        return ((char*)p+sizeof(MemoryUnit));
    }

    return _blockArray[size/8]->alloc();
}

void MemoryPool::free(void *p){
    MemoryUnit* memoryUnit=(MemoryUnit*)((char*)p-sizeof(MemoryUnit));
    if(memoryUnit->holder)
        memoryUnit->holder->dealloc(p);
    else
        ::free(memoryUnit);
}

