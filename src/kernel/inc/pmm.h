#ifndef PMM_H
#define PMM_H

#include <util.h>
u64* pmmAlloc(u64 l);
void pmmFree(u64 addr, u64 l);
void pmmInit();
#endif