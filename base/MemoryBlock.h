//
// Created by zhsy on 19-7-9.
//

#ifndef HCCSERVER_MEMORYBLOCK_H
#define HCCSERVER_MEMORYBLOCK_H

#include <mutex>
#include <cstddef>
#include<malloc.h>
#include <cassert>

class MemoryBlock;
struct MemoryUnit{
    MemoryBlock *holder;
    MemoryUnit *next;
};
/*
 * 一个块中（MemoryBlock）有多个内存元（MemoryUnit）
 *
 */
class MemoryBlock {
public:
    MemoryBlock(int unitSize):_unitSize(unitSize),_restNum(0),_capacity(0),_nextBlock(nullptr){

    }
    ~MemoryBlock(){

    }

    void *alloc(){
        std::lock_guard<std::mutex> l(_mutex);
        if(_nextBlock== nullptr){
            expandBlock();
        }

        assert(_nextBlock!= nullptr);

        char *p=(char*)_nextBlock;

        _nextBlock=_nextBlock->next;

        _restNum--;

        return p+sizeof(MemoryUnit);
    }
    void dealloc(void* p){
        std::lock_guard<std::mutex> l(_mutex);
        MemoryUnit* memoryUnit=(MemoryUnit*)((char*)p-sizeof(MemoryUnit));

        assert(memoryUnit->holder==this);

        memoryUnit->next=_nextBlock;
        _nextBlock=memoryUnit;
        _restNum++;
    }
    void* operator new(size_t size){
        return ::malloc(size);
    }
    void operator delete(void* p){
        ::free(p);
    }
private:
    int _unitSize;
    size_t _restNum;
    size_t _capacity;
    MemoryUnit *_nextBlock;
    std::mutex _mutex;
private:
    void expandBlock(){
        if(_capacity==0){
            //初次使用，分配初始空间
            initMemoryUnit();
            return;
        }
        assert(_nextBlock== nullptr);

        //分配内存资源
        char* block=(char*)malloc((_unitSize+sizeof(MemoryUnit))*_capacity);
        //初始化内存单元
        int i=0;
        for(;i<_capacity-1;i++){
            MemoryUnit* p=(MemoryUnit*)(block+i*(_unitSize+sizeof(MemoryUnit)));
            p->holder=this;
            p->next=(MemoryUnit*)(block+(i+1)*(_unitSize+sizeof(MemoryUnit)));
        }
        MemoryUnit* p=(MemoryUnit*)(block+i*(_unitSize+sizeof(MemoryUnit)));
        p->holder=this;
        p->next= nullptr;
        _nextBlock=(MemoryUnit*)block;
        _restNum+=_capacity;
        _capacity*=2;
    }
    void initMemoryUnit(){
        assert(_capacity==0);
        _capacity=10;
        //分配内存资源
        char* block=(char*)malloc((_unitSize+sizeof(MemoryUnit))*_capacity);
        //初始化内存单元
        int i=0;
        for(;i<_capacity-1;i++){
            MemoryUnit* p=(MemoryUnit*)(block+i*(_unitSize+sizeof(MemoryUnit)));
            p->holder=this;
            p->next=(MemoryUnit*)(block+(i+1)*(_unitSize+sizeof(MemoryUnit)));
        }
        MemoryUnit* p=(MemoryUnit*)(block+i*(_unitSize+sizeof(MemoryUnit)));
        p->holder=this;
        p->next= nullptr;
        _nextBlock=(MemoryUnit*)block;
        _restNum=10;
    }
};


#endif //HCCSERVER_MEMORYBLOCK_H
