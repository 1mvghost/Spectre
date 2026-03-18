#ifndef ALLOC_H
#define ALLOC_H
#include <util.h>
void* malloc(size_t size);
void free(void* addr);
void* calloc(size_t size);
void* realloc(void* addr, size_t size);
void spdmp();
#endif