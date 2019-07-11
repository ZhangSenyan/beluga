//
// Created by zhsy on 19-7-10.
//

#ifndef HCCSERVER_OBJECTPOOL_H
#define HCCSERVER_OBJECTPOOL_H

#include <cstdio>
#include <mutex>
#include <cassert>
#include <iostream>
template <class T>
class ObjectPool{
public:
    ObjectPool();
    ~ObjectPool();
    T *alloc();
    void dealloc(T* p);
private:
    int _unitSize;
    size_t _restNum;
    size_t _capacity;
    T *_nextObj;
private:
    void expandBlock();
    void initMemoryUnit();
};

template <class T>
ObjectPool<T>::ObjectPool():_nextObj(nullptr),_restNum(0),_capacity(0)
{
    std::cout<<"ObjectPool<T>::ObjectPool()"<<std::endl;
}
template <class T>
ObjectPool<T>::~ObjectPool(){

}
template <class T>
T *ObjectPool<T>::alloc(){

    if(_nextObj== nullptr){
        expandBlock();
    }
    assert(_nextObj!= nullptr);
    T *ret=_nextObj;
    _nextObj=_nextObj->_next;
    _restNum--;
    return ret;
}
template <class T>
void ObjectPool<T>::dealloc(T* p){
    p->destory();
    p->_next=_nextObj;
    _nextObj=p;
    _restNum++;
}


template <class T>
void ObjectPool<T>::expandBlock(){
    if(_capacity==0){
        //初次使用，分配初始空间
        initMemoryUnit();
        return;
    }
    assert(_nextObj== nullptr);

    T * ObjSet=new T[_capacity];

    int i=0;
    for(;i<_capacity-1;i++){
        ObjSet[i]._next=&ObjSet[i+1];
    }
    ObjSet[i]._next=nullptr;
    _nextObj=&ObjSet[0];
    _restNum+=_capacity;
    _capacity*=2;
}

template <class T>
void ObjectPool<T>::initMemoryUnit(){
    assert(_capacity==0);
    _capacity=10;
    T * ObjSet=new T[_capacity];

    int i=0;
    for(;i<_capacity-1;i++){
        ObjSet[i]._next=&ObjSet[i+1];
    }
    ObjSet[i]._next=nullptr;
    _nextObj=&ObjSet[0];
    _restNum+=_capacity;
}


#endif //HCCSERVER_OBJECTPOOL_H
