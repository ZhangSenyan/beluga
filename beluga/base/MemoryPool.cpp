/**
 * @author Zhang Senyan
 * Date: 2019-06-07
 *
 */

#include <iostream>
#include "MemoryPool.h"

//构造函数
MemoryPool::MemoryPool(){

    // 申请16个内存块，放入池中
    for(int i=0;i<16;i++){
        _blockArray[i]=new MemoryBlock((i+1)*8);
    }

}

//析构函数
MemoryPool::~MemoryPool(){

}

//单例-饿汉
MemoryPool& MemoryPool::instance(){
    static MemoryPool memoryPool;
    return memoryPool;
}

/**
 * 申请指定大小内存
 * 不需要加锁，锁在单个内存块中，一定程度能够缓解锁的争用
 *
 *  @param : size 申请内存的大小
 *
 *  @return ： 申请内存首地址
 *
 */
void* MemoryPool::alloc(size_t size){

    if(size>=128){
        //向系统申请内存
        MemoryUnit* p= (MemoryUnit*) (malloc(size+sizeof(MemoryUnit)));

        //所属的内存块为空
        p->holder= nullptr;
        p->next= nullptr;

        //后移8个字节，返回指针
        return ((char*)p+sizeof(MemoryUnit));
    }

    /**
     * 向内存池申请内存
     * size/8 -> 确定目标内存块
     * 向目标内存块申请内存
     */
    return _blockArray[size/8]->alloc();
}

/**
 * 释放内存块
 *
 *  @param : void* 要释放的内存首地址
 *
 */
void MemoryPool::free(void *p){
    //指针前移，获取头信息
    MemoryUnit* memoryUnit=(MemoryUnit*)((char*)p-sizeof(MemoryUnit));

    if(memoryUnit->holder)
        //放到内存块中
        memoryUnit->holder->dealloc(p);
    else
        //释放内存
        ::free(memoryUnit);
}

