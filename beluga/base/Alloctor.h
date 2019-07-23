/**
 * @author Zhang Senyan
 * Date: 19/05/2019
 *
 * @details 内存池相关接口函数声明
 *
 */

#ifndef _ALLOCTOR_H
#define _ALLOCTOR_H

#include <cstddef>

void* operator new(size_t size);

void operator delete(void* p) noexcept;

void* operator new[](size_t size);

void operator delete[](void* p);


#endif