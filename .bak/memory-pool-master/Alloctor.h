#ifndef _ALLOCTOR_H
#define _ALLOCTOR_H

void* operator new(size_t size);
void operator delete(void* p);
void* operator new[](size_t size);
void operator delete[](void* p);
void* mem_alloc(size_t size);
void mem_alloc(void* p);
#endif