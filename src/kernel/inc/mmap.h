#ifndef MMAP_H
#define MMAP_H
#include <limine.h>
#include <util.h>

void mMapInit();
struct limine_memmap_entry mMapGet(u64 i);
int mMapLen();
u64 mMapTotalMem();

#endif