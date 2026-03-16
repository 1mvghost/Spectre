#ifndef ALLOC_H
#define ALLOC_H
#include <util.h>
void* spmalloc(size_t size);
void spfree(void* addr);
void* spcalloc(size_t size);
void* sprealloc(void* addr, size_t size);
void spdmp();
#endif