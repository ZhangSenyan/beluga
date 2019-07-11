//
// Created by zhsy on 19-6-8.
//

#ifndef HCCSERVER_MEMORYPOOL_H
#define HCCSERVER_MEMORYPOOL_H

#include "MemoryBlock.h"

class MemoryPool {
private:
    MemoryPool();
    ~MemoryPool();
public:
    static MemoryPool& instance();
    void* alloc(size_t size);
    void free(void *p);
    void* operator new(size_t size);
    void operator delete(void* p) noexcept ;
private:
    MemoryBlock* _blockArray[16];


};


#endif //HCCSERVER_MEMORYPOOL_H
