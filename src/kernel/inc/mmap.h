#ifndef MMAP_H
#define MMAP_H
#include <util.h>
#include <limine.h>
void mMapInit(struct limine_memmap_response *mMap);
struct limine_memmap_entry mMapGet(u64 i);
int mMapLen();
#endif