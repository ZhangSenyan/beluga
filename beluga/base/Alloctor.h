#ifndef _ALLOCTOR_H
#define _ALLOCTOR_H

#include <cstddef>

void* operator new(size_t size);
void operator delete(void* p) noexcept;
void* operator new[](size_t size);
void operator delete[](void* p);
void* mem_alloc(size_t size);
void mem_alloc(void* p);

#endif