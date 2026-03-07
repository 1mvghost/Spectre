#ifndef PMM_H
#define PMM_H

#include <util.h>
u64* pmmAlloc();
void pmmFree(u64 addr);
void pmmInit();
#endif