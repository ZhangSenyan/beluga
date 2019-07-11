//
// Created by zhsy on 19-7-9.
//

#ifndef HCCSERVER_MEMORYBLOCK_H
#define HCCSERVER_MEMORYBLOCK_H

#include <mutex>


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
    explicit MemoryBlock(int unitSize);
    ~MemoryBlock();

    void *alloc();
    void dealloc(void* p);
    void* operator new(size_t size);
    void operator delete(void* p);
private:
    int _unitSize;
    size_t _restNum;
    size_t _capacity;
    MemoryUnit *_nextBlock;
    std::mutex _mutex;
private:
    void expandBlock();
    void initMemoryUnit();
};


#endif //HCCSERVER_MEMORYBLOCK_H
