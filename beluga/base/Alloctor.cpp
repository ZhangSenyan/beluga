
/**
 * @author Zhang Senyan
 * Date: 19/05/2019
 *
 * @details 内存池相关接口函数
 * 如果要开启内存池，需要  #define USE_MEMORY_POOL
 */

#include <iostream>

#include"MemoryPool.h"
#include"Alloctor.h"

//#define USE_MEMORY_POOL
#ifdef USE_MEMORY_POOL

// 重载 operator new
void* operator new(size_t size)
{
    return MemoryPool::instance().alloc(size);
}

// 重载 operator delete
void operator delete(void* p) noexcept
{
    return MemoryPool::instance().free(p);
}

// 重载 operator new[]
void* operator new[](size_t size)
{
    return MemoryPool::instance().alloc(size);
}

// 重载 operator delete[]
void operator delete[](void* p) noexcept
{
    return MemoryPool::instance().free(p);
}

#endif
