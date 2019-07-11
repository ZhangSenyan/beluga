#include <iostream>
#include"MemoryPool.h"

#include"Alloctor.h"

void* operator new(size_t size)
{
    //std::cout<<"operator new size="<<size<<std::endl;
    return MemoryPool::instance().alloc(size);
}
void operator delete(void* p) noexcept
{
    //std::cout<<"operator delete"<<std::endl;
    return MemoryPool::instance().free(p);
}

void* operator new[](size_t size)
{
    //std::cout<<"MemoryPool::instance().alloc(size)="<<size<<std::endl;
    return MemoryPool::instance().alloc(size);
}
void operator delete[](void* p) noexcept
{
    //std::cout<<"operator delete[]"<<std::endl;
    return MemoryPool::instance().free(p);
}
void* mem_alloc(size_t size)
{
	return malloc(size);
}
void mem_alloc(void* p)
{
	free(p);
}
