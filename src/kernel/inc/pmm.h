#ifndef PMM_H
#define PMM_H

#include <util.h>
u64* pmmAlloc();
void pmmInit();
void pmmFree(u64 addr);
#endif