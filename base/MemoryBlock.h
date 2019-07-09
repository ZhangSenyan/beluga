//
// Created by zhsy on 19-7-9.
//

#ifndef HCCSERVER_MEMORYBLOCK_H
#define HCCSERVER_MEMORYBLOCK_H

#include <cstddef>


class MemoryBlock {
public:
    MemoryBlock(int size):_baseSize(size){

    }
    ~MemoryBlock(){

    }

private:
    int _baseSize;

};


#endif //HCCSERVER_MEMORYBLOCK_H
