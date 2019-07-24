/**
 * @author Zhang Senyan
 * Date: 2019-06-05
 *
 * 内存块：存放固定规格大小的内存单元
 */

#ifndef BELUGA_MEMORYBLOCK_H
#define BELUGA_MEMORYBLOCK_H

#include <mutex>


class MemoryBlock;

/**
 * 内存单元的头部
 * 数据部分不需要声明
 */
struct MemoryUnit{
    /**
     * 内存单元所属的内存块
     * 归还内存时使用
     */
    MemoryBlock *holder;

    //指向池中的下一个内存单元
    MemoryUnit *next;

    //data ...
};

/**
 * 内存块：里面包含指定规格的内存单元
 * Lazy加载模式
 *
 * 由于内存块构造时，不能从内存池拿内存，需要从系统中取内存
 * 所以需要在其内部指定 operator new
 */
class MemoryBlock {
public:
    //构造函数
    explicit MemoryBlock(int unitSize);

    //析构函数
    ~MemoryBlock();

    //取出一个内存单元
    void *alloc();

    //归还一个内存单元
    void dealloc(void* p);

    void* operator new(size_t size);

    void operator delete(void* p);

private:
    //内存单元规格大小
    int _unitSize;

    //剩余内存单元数量
    size_t _restNum;

    //内存块容量
    size_t _capacity;

    //栈顶内存单元
    MemoryUnit *_nextBlock;

    //内存锁，每个内存块都有一个单独的锁
    std::mutex _mutex;
private:
    //剩余内存单元不足时，拓展内存
    void expandBlock();

    //初始化内存单元
    void initMemoryUnit();
};


#endif //BELUGA_MEMORYBLOCK_H
