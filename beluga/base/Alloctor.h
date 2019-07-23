/**
 * @author Zhang Senyan
 * Date: 19/05/2019
 *
 * @details 内存池相关接口函数声明
 *
 */

#ifndef BELUGA_ALLOCTOR_H
#define BELUGA_ALLOCTOR_H

#include <cstddef>

void* operator new(size_t size);

void operator delete(void* p) noexcept;

void* operator new[](size_t size);

void operator delete[](void* p);


#endif //BELUGA_ALLOCTOR_H