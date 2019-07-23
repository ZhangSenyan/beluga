/**
 * @author Zhang Senyan
 * Date: 2019-06-07
 *
 * 内存池：
 *    包含16种规格 (1-16)×8 的内存块
 *    size：申请内存的大小
 *    if size>128:
 *      调用系统函数malloc
 *    else:
 *      挑选内存块
 *      申请一个内存单元
 * 单例-饿汉
 */


#ifndef BELUGA_MEMORYPOOL_H
#define BELUGA_MEMORYPOOL_H

#include "MemoryBlock.h"

class MemoryPool {
private:
    //构造函数 - 单例
    MemoryPool();

    //析构函数
    ~MemoryPool();

public:
    // get 内存池
    static MemoryPool& instance();

    // 申请大小为 size 的内存
    void* alloc(size_t size);

    //释放 p 指向的内存
    void free(void *p);

    //构造MemoryPool时需要调用系统malloc
    void* operator new(size_t size);

    //析构MemoryPool时需要调用系统free
    void operator delete(void* p) noexcept ;

private:

    //内存块池
    MemoryBlock* _blockArray[16];

};


#endif //BELUGA_MEMORYPOOL_H


