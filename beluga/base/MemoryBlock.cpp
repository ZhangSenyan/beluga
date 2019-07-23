/**
 * @author Zhang Senyan
 * Date: 2019-06-05
 *
 */



#include <cstddef>
#include <malloc.h>
#include <cassert>

#include "beluga/base/MemoryBlock.h"

//构造函数
MemoryBlock::MemoryBlock(int unitSize):
            _unitSize(unitSize),
            _restNum(0),
            _capacity(0),
            _nextBlock(nullptr){

}

//分配一个内存单元
void *MemoryBlock::alloc(){

    std::lock_guard<std::mutex> l(_mutex);

    if(_nextBlock== nullptr){
        //当内存单元不足时，拓展内存单元数量
        expandBlock();
    }

    assert(_nextBlock!= nullptr);

    //取出栈顶内存单元
    char *p=(char*)_nextBlock;

    //获取下一个内存单元
    _nextBlock=_nextBlock->next;
    _restNum--;

    /**
     * struct of MemoryUnit
     * |  *holder   |    *next  |         data ...          |
     * |<--sizeof(MemoryUnit)-->|
     * |<-p                     |<-p+sizeof(MemoryUnit)
     *
     * 所以指针返回时 应该向后移动8个字节
     */
    return p+sizeof(MemoryUnit);
}

//归还一个内存单元
void MemoryBlock::dealloc(void* p){
    std::lock_guard<std::mutex> l(_mutex);

    // 归还时指针向前移动 sizeof(MemoryUnit) 个字节
    MemoryUnit* memoryUnit=(MemoryUnit*)((char*)p-sizeof(MemoryUnit));

    assert(memoryUnit->holder==this);

    //内存单元入栈
    memoryUnit->next=_nextBlock;
    _nextBlock=memoryUnit;
    _restNum++;
}

//构造内存块时调用此函数
void* MemoryBlock::operator new(size_t size){
    //直接向系统申请内存
    return ::malloc(size);
}

//析构内存块时调用此函数
void MemoryBlock::operator delete(void* p){
    ::free(p);
}

/**
 * 拓展内存单元的数量
 *
 * 2倍扩容机制：
 * -> 初始分配内存时，分配10个单元
 * -> 之后每次用尽都将容量拓展为之前的两倍
 *
 */
void MemoryBlock::expandBlock(){
    if(_capacity==0){
        //初次使用，分配初始空间
        initMemoryUnit();
        return;
    }

    //非初次使用，且剩余内存单元为空
    assert(_nextBlock== nullptr);

    /**
     * _unitSize: 要扩充的内存单元的规格
     * sizeof(MemoryUnit)： 附加的内存单元头信息所占用空间的大小
     * _capacity：再次申请内存的数量，等于上次内存的容量
     *            扩充之后内存容量变为原来的2倍
     */
    char* block=(char*)malloc((_unitSize+sizeof(MemoryUnit))*_capacity);

    //对刚申请的内存单元初始化
    int i=0;
    for(;i<_capacity-1;i++){

        MemoryUnit* p=(MemoryUnit*)(block+i*(_unitSize+sizeof(MemoryUnit)));

        //赋值，所属的内存块
        p->holder=this;

        //通过指针将所有内存单元串接起来
        p->next=(MemoryUnit*)(block+(i+1)*(_unitSize+sizeof(MemoryUnit)));
    }

    MemoryUnit* p=(MemoryUnit*)(block+i*(_unitSize+sizeof(MemoryUnit)));
    p->holder=this;

    //最后一个内存单元指向 nullptr
    p->next= nullptr;

    //将栈顶移动到该内存单元的头部
    _nextBlock=(MemoryUnit*)block;

    _restNum+=_capacity;
    _capacity*=2;
}

/**
 * 初始化内存块
 *
 * 仅在首次访问该内存块时调用
 * 分配内存单元数量为10
 *
 */
void MemoryBlock::initMemoryUnit(){

    assert(_capacity==0);
    _capacity=10;

    //申请相应大小的内存
    char* block=(char*)malloc((_unitSize+sizeof(MemoryUnit))*_capacity);

    //内存单元初始化
    int i=0;
    for(;i<_capacity-1;i++){
        MemoryUnit* p=(MemoryUnit*)(block+i*(_unitSize+sizeof(MemoryUnit)));

        //记录所属的内存块，并将各个内存单元串接起来
        p->holder=this;
        p->next=(MemoryUnit*)(block+(i+1)*(_unitSize+sizeof(MemoryUnit)));
    }
    MemoryUnit* p=(MemoryUnit*)(block+i*(_unitSize+sizeof(MemoryUnit)));
    p->holder=this;
    p->next= nullptr;

    //将内存栈顶指向刚申请的内存块
    _nextBlock=(MemoryUnit*)block;
    _restNum=10;
}