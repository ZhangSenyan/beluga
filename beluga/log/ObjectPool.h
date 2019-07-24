/**
 * @author Zhang Senyan
 * Date: 2019-06-25
 *
 * 对象池
 */

#ifndef BELUGA_OBJECTPOOL_H
#define BELUGA_OBJECTPOOL_H

#include <cstdio>
#include <mutex>
#include <cassert>
#include <iostream>


template <class T>
class ObjectPool{
public:
    //构造函数
    ObjectPool();
    ~ObjectPool();

    //申请一个对象
    T *alloc();

    //归还一个对象
    void dealloc(T* p);

private:

    //池中剩余对象数量
    size_t _restNum;

    //对象池容量
    size_t _capacity;

    //对象池栈顶
    T *_nextObj;
private:

    /**
     * 扩展对象池
     * 当对象数量不足时调用
     */
    void expandBlock();

    /**
     * lazy 加载模式
     * 第一次使用对象池时调用
     */
    void initMemoryUnit();
};

//构造函数
template <class T>
ObjectPool<T>::ObjectPool():
            _nextObj(nullptr),
            _restNum(0),
            _capacity(0){

}

template <class T>
ObjectPool<T>::~ObjectPool(){

}

//申请一个对象
template <class T>
T *ObjectPool<T>::alloc(){

    /**
     * _nextObj 为空，没有剩余对象
     * 需要拓宽对象池容量
     */
    if(_nextObj== nullptr){
        expandBlock();
    }

    assert(_nextObj!= nullptr);

    //取出栈顶元素
    T *ret=_nextObj;

    //调节栈指针
    _nextObj=_nextObj->_next;
    _restNum--;
    return ret;
}

/**
 * 将对象归还对象池
 * @tparam T 对象类型
 * @param p 对象指针
 */
template <class T>
void ObjectPool<T>::dealloc(T* p){

    //销毁对象
    p->destory();

    //对象入栈
    p->_next=_nextObj;

    //调节栈指针
    _nextObj=p;
    _restNum++;
}

/**
 * 对象池扩容
 * 对象池中对象用光，需要二倍扩容
 */
template <class T>
void ObjectPool<T>::expandBlock(){

    if(_capacity==0){
        //初次使用，分配初始空间
        initMemoryUnit();
        return;
    }

    assert(_nextObj== nullptr);

    //申请对象
    T * ObjSet=new T[_capacity];

    //将对象串接起来
    int i=0;
    for(;i<_capacity-1;i++){
        ObjSet[i]._next=&ObjSet[i+1];
    }

    //最后的一个对象指针设为空
    ObjSet[i]._next=nullptr;

    //调节栈顶指针
    _nextObj=&ObjSet[0];
    _restNum+=_capacity;
    _capacity*=2;
}

/**
 * 初始化对象池
 * 分配10个对象大小容量
 */
template <class T>
void ObjectPool<T>::initMemoryUnit(){
    assert(_capacity==0);
    _capacity=10;

    //申请对象
    T * ObjSet=new T[_capacity];

    //将对象串接在一起
    int i=0;
    for(;i<_capacity-1;i++){
        ObjSet[i]._next=&ObjSet[i+1];
    }
    ObjSet[i]._next=nullptr;

    //调节栈顶指针
    _nextObj=&ObjSet[0];
    _restNum+=_capacity;
}


#endif //BELUGA_OBJECTPOOL_H
